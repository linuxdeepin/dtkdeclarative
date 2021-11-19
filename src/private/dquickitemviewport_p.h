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

#include <QSGImageNode>
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
            // 在边缘额外加一个像素，用于 ClampToEdge 时不会取到边缘的透明像素点
            QImage mask(QSize(qRound(radius) + 1, qRound(radius) + 1), QImage::Format_ARGB32);
            mask.fill(Qt::transparent);
            // 必须填充为白色，在着色器中运算时会使用rgb三个通道相乘
            const QColor maskColor = Qt::white;
            QPainter pa(&mask);
            pa.setPen(maskColor);
            const QRect r = mask.rect();
            pa.drawLine(r.bottomLeft(), r.bottomRight());
            pa.drawLine(r.topRight(), r.bottomRight());
            pa.setRenderHint(QPainter::Antialiasing, antialiasing);
            QPainterPath path;
            path.moveTo(radius, radius);
            path.arcTo(0, 0, radius * 2,  radius * 2, 90, 90);
            path.lineTo(radius, radius);
            path.closeSubpath();
            pa.fillPath(path, maskColor);
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
class PreprocessNode;
class Q_DECL_HIDDEN DQuickItemViewportPrivate : public DCORE_NAMESPACE::DObjectPrivate, public QQuickItemChangeListener
{
public:
    enum DirtyStateBit {
        DirtyNothing = 0x0,
        DirtySourceSizeRatio = 0x1,
        DirtyMaskTexture = 0x2,
        DirtyMaskSizeRatio = 0x4,
        DirtyMaskOffset = 0x8,
        DirtyContentNode = 0x10
    };
    Q_DECLARE_FLAGS(DirtyState, DirtyStateBit)

    DQuickItemViewportPrivate(DQuickItemViewport *qq)
        : DObjectPrivate(qq)
    {

    }

    ~DQuickItemViewportPrivate() override;

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
        markDirty(DirtySourceSizeRatio, false);
        const auto &sr = getSourceRect();
        soureSizeRatio.setX(static_cast<float>(sourceItem->width() / sr.width()));
        soureSizeRatio.setY(static_cast<float>(sourceItem->height() / sr.height()));
        return soureSizeRatio;
    }
    inline const QVector2D &getMaskSizeRatio() {
        if (Q_LIKELY(!dirtyState.testFlag(DirtyMaskSizeRatio))) {
            return maskSizeRatio;
        }

        Q_ASSERT(radius > 0);
        markDirty(DirtyMaskSizeRatio, false);
        const auto &sr = getSourceRect();
        maskSizeRatio.setX(static_cast<float>(sr.width() / static_cast<qreal>(radius)));
        maskSizeRatio.setY(static_cast<float>(sr.height() / static_cast<qreal>(radius)));
        return maskSizeRatio;
    }
    inline const QVector2D &getMaskOffset() {
        if (Q_LIKELY(!dirtyState.testFlag(DirtyMaskOffset))) {
            return maskOffset;
        }

        Q_ASSERT(sourceItem && sourceItem->width() > 0 && sourceItem->height() > 0);
        markDirty(DirtyMaskOffset, false);
        auto offset = getSourceRect().topLeft();
        maskOffset.setX(static_cast<float>(offset.x() / sourceItem->width()));
        maskOffset.setY(static_cast<float>(offset.y() / sourceItem->height()));
        return maskOffset;
    }

    inline QSGTexture *textureForRadiusMask()
    {
        Q_ASSERT(radius > 0);
        if (Q_UNLIKELY(dirtyState.testFlag(DirtyMaskTexture) || !maskTexture)) {
            QQuickItemPrivate *d = QQuickItemPrivate::get(q_func());
            maskTexture = MaskTextureCache::instance()->getTexture(d->sceneGraphRenderContext(),
                                                                   static_cast<float>(radius * d->window->effectiveDevicePixelRatio()),
                                                                   true);

            markDirty(DirtyMaskTexture, false);
        }

        return maskTexture->texture;
    }

    inline bool needMaskNode() const {
        return radius > 0;
    }

    inline bool updateOffset(const QPointF &offset) {
        if (this->offset == offset)
            return false;
        this->offset = offset;
        markDirty(DirtyMaskOffset);
        return true;
    }

    inline QRectF getSourceRect() const {
        QRectF sr = sourceRect;
        if (!sourceRect.isValid()) {
            sr = QRectF(QPointF(0, 0), q_func()->size());
        }

        return fixed ? sr : sr.translated(offset);
    }

    inline void updateSourceRect(QSGImageNode *imageNode) const {
        const QSizeF &textureSize = imageNode->texture()->textureSize();
        qreal xScale = textureSize.width() / sourceItem->width();
        qreal yScale = textureSize.height() / sourceItem->height();
        // 计算sourceItem应该被绘制的区域，如果此区域大小为0, 则没有必要再继续绘制
        const QRectF &sourceRect = getSourceRect();
        // 更新 DQuickItemViewport 所对应的sourceItem的材质区域
        imageNode->setSourceRect(QRectF(sourceRect.x() * xScale, sourceRect.y() * yScale,
                                        sourceRect.width() * xScale, sourceRect.height() * yScale));
    }

    void setPreprocessNode(PreprocessNode *newNode);
    void clearPreprocessNode(PreprocessNode *oldNode);
    void updateUsePreprocess() const;

    D_DECLARE_PUBLIC(DQuickItemViewport)

    QPointer<QQuickItem> sourceItem;
    QAtomicPointer<PreprocessNode> preprocessNode;
    // 记录sourceItem的大小是自身的多少倍
    QVector2D soureSizeRatio;
    // 显示圆角的mask材质
    MaskTextureCache::TextureData maskTexture;
    // item自身相对于圆角大小的比例
    QVector2D maskSizeRatio;
    // mask材质相对于sourceItem材质的偏移量
    QVector2D maskOffset;
    QMetaObject::Connection textureChangedConnection;
    // 自身位置相对于sourceItem的偏移量
    QPointF offset = QPointF(0, 0);
    QRectF sourceRect;
    // 记录待更新的数据类型
    DirtyState dirtyState = DirtyNothing;
    // 圆角半径大小
    float radius = 0;
    bool fixed = false;
};
Q_DECLARE_OPERATORS_FOR_FLAGS(DQuickItemViewportPrivate::DirtyState)
DQUICK_END_NAMESPACE
#endif // DQUICKITEMVIEWPORT_P_H
