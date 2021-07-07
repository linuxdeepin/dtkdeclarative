/*
 * Copyright (C) 2020 Uniontech Technology Co., Ltd.
 *
 * Author:     xiaoyaobing <xiaoyaobing@uniontech.com>
 *
 * Maintainer: xiaoyaobing <xiaoyaobing@uniontech.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef DQUICKITEMVIEWPORT_P_H
#define DQUICKITEMVIEWPORT_P_H

#include "dquickitemviewport.h"

#include <DObjectPrivate>

#include <private/qsgadaptationlayer_p.h>
#include <private/qquickitem_p.h>
#include <private/qquickitemchangelistener_p.h>

class Q_DECL_HIDDEN MaskTextureCache {
public:
    class Texture : public QSharedData {
    public:
        Texture(QSGTexture *t)
            : texture(t)
        {
            MaskTextureCache::instance()->m_cache[cacheKey] = this;
        }

        ~Texture() {
            MaskTextureCache::instance()->m_cache.remove(cacheKey);
            delete texture;
        }

        qint8 cacheKey;
        QSGTexture *texture;
    };

    typedef QExplicitlySharedDataPointer<Texture> TextureData;

    static MaskTextureCache *instance()
    {
        static MaskTextureCache *object = new MaskTextureCache();
        return object;
    }

    // 根据圆角大小获取一个蒙版材质，此材质将用于片段着色器中实现圆角效果
    TextureData getTexture(QSGRenderContext *context, float radius, bool antialiasing)
    {
        // 排除无效的数据
        if (qIsNull(radius))
            return TextureData();

        // 用于获取材质缓存key的key
        qint8 to_cache_key_key = ((antialiasing << 7) | qRound(radius));
        Texture *texture = nullptr;

        if (m_radiusToCacheKey.contains(to_cache_key_key)) {
            texture = m_cache.value(m_radiusToCacheKey.value(to_cache_key_key));
        }

        if (!texture) {
            QImage mask(QSize(qRound(radius), qRound(radius)), QImage::Format_ARGB32);
            mask.fill(Qt::transparent);
            QPainter pa(&mask);
            pa.setRenderHint(QPainter::Antialiasing, antialiasing);
            QPainterPath path;
            path.moveTo(radius, radius);
            path.arcTo(0, 0, radius * 2,  radius * 2, 90, 90);
            path.lineTo(radius, radius);
            path.closeSubpath();
            // 必须填充为白色，在着色器中运算时会使用rgb三个通道相乘
            pa.fillPath(path, Qt::white);
            pa.end();

            texture = new Texture(context->createTexture(mask));
            texture->texture->setFiltering(QSGTexture::Nearest);
            texture->texture->setVerticalWrapMode(QSGTexture::ClampToEdge);
            texture->texture->setHorizontalWrapMode(QSGTexture::ClampToEdge);

            m_radiusToCacheKey[to_cache_key_key] = texture->cacheKey;
        }

        // 为窗口保存mask材质
        TextureData data(texture);

        return data;
    }

private:
    MaskTextureCache() {

    }

    QHash<qint8, Texture*> m_cache;
    QMap<int, qint8> m_radiusToCacheKey;

    friend class Texture;
};

DQUICK_BEGIN_NAMESPACE
class Q_DECL_HIDDEN DQuickItemViewportPrivate : public DCORE_NAMESPACE::DObjectPrivate, public QQuickItemChangeListener
{
public:
    enum DirtyStateBit {
        DirtyNothing = 0x0,
        DirtySourceTexture = 0x1,
        DirtySourceSizeRatio = 0x2,
        DirtyMaskTexture = 0x4,
        DirtyMaskSizeRatio = 0x8,
        DirtyMaskOffset = 0x10
    };
    Q_DECLARE_FLAGS(DirtyState, DirtyStateBit)

    DQuickItemViewportPrivate(DQuickItemViewport *qq)
        : DObjectPrivate(qq)
    {

    }

    ~DQuickItemViewportPrivate() override
    {
        // 清理对sourceItem的操作
        initSourceItem(sourceItem, nullptr);
    }

    inline void markDirtys(DirtyState states) {
        dirtyState |= states;
    }
    inline void markDirty(DirtyStateBit state, bool dirty = true) {
        if (dirty) {
            dirtyState |= state;
        } else {
            dirtyState &= ~state;
        }
    }

    // 根据radius获取对应的蒙版材质
    void initSourceItem(QQuickItem *old, QQuickItem *item);

    void itemGeometryChanged(QQuickItem *item, QQuickGeometryChange data, const QRectF &) override;

    inline const QVector2D &getSoureSizeRatio() {
        if (Q_LIKELY(!dirtyState.testFlag(DirtySourceSizeRatio))) {
            return soureSizeRatio;
        }

        Q_ASSERT(sourceItem);
        D_Q(const DQuickItemViewport);
        markDirty(DirtySourceSizeRatio, false);
        soureSizeRatio.setX(static_cast<float>(sourceItem->width() / q->width()));
        soureSizeRatio.setY(static_cast<float>(sourceItem->height() / q->height()));
        return soureSizeRatio;
    }
    inline const QVector2D &getMaskSizeRatio() {
        if (Q_LIKELY(!dirtyState.testFlag(DirtyMaskSizeRatio))) {
            return maskSizeRatio;
        }

        Q_ASSERT(radius > 0);
        D_Q(const DQuickItemViewport);
        markDirty(DirtyMaskSizeRatio, false);
        maskSizeRatio.setX(static_cast<float>(q->width() / static_cast<qreal>(radius)));
        maskSizeRatio.setY(static_cast<float>(q->height() / static_cast<qreal>(radius)));
        return maskSizeRatio;
    }
    inline const QVector2D &getMaskOffset() {
        if (Q_LIKELY(!dirtyState.testFlag(DirtyMaskOffset))) {
            return maskOffset;
        }

        Q_ASSERT(sourceItem && sourceItem->width() > 0 && sourceItem->height() > 0);
        D_Q(const DQuickItemViewport);
        markDirty(DirtyMaskOffset, false);
        auto offset = q->position() + sourceOffset;
        maskOffset.setX(static_cast<float>(offset.x() / sourceItem->width()));
        maskOffset.setY(static_cast<float>(offset.y() / sourceItem->height()));
        return maskOffset;
    }

    inline QSGTexture *textureForRadiusMask()
    {
        if (Q_UNLIKELY(dirtyState.testFlag(DirtyMaskTexture) || !maskTexture)) {
            QQuickItemPrivate *d = QQuickItemPrivate::get(q_func());
            maskTexture = MaskTextureCache::instance()->getTexture(d->sceneGraphRenderContext(),
                                                                   static_cast<float>(radius * d->window->effectiveDevicePixelRatio()),
                                                                   true);
            markDirty(DirtyMaskTexture, false);
        }

        return maskTexture->texture;
    }

    D_DECLARE_PUBLIC(DQuickItemViewport)

    QPointer<QQuickItem> sourceItem;
    // 记录sourceItem的大小是自身的多少倍
    QVector2D soureSizeRatio;
    // 显示圆角的mask材质
    MaskTextureCache::TextureData maskTexture;
    // item自身相对于圆角大小的比例
    QVector2D maskSizeRatio;
    // mask材质相对于sourceItem材质的偏移量
    QVector2D maskOffset;
    QMetaObject::Connection textureChangedConnection;
    // 自身parentItem位置相对于sourceItem的偏移量
    QPointF sourceOffset = QPointF(0, 0);
    // 记录待更新的数据类型
    DirtyState dirtyState = DirtyNothing;
    // 圆角半径大小
    float radius = 0;
    QColor foregroundColor;

    static QQuickItemPrivate::ChangeType changeType;
};
Q_DECLARE_OPERATORS_FOR_FLAGS(DQuickItemViewportPrivate::DirtyState)
DQUICK_END_NAMESPACE
#endif // DQUICKITEMVIEWPORT_P_H
