/*
 * Copyright (C) 2020 Uniontech Technology Co., Ltd.
 *
 * Author:     zccrs <zccrs@live.com>
 *
 * Maintainer: zccrs <zhangjide@uniontech.com>
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

#include "dquickitemviewport.h"
#include "private/dquickitemviewport_p.h"
#include "private/dquickmaskeffectnode.h"

#include <QOpenGLFunctions>
#include <QQuickImageProvider>

DQUICK_BEGIN_NAMESPACE

QQuickItemPrivate::ChangeType DQuickItemViewportPrivate::changeType = QQuickItemPrivate::Geometry;

void DQuickItemViewportPrivate::initSourceItem(QQuickItem *old, QQuickItem *item)
{
    if (old) {
        QQuickItemPrivate *sd = QQuickItemPrivate::get(old);
        sd->removeItemChangeListener(this, QQuickItemPrivate::Geometry);
    }

    // 监听材质变化的信号
    if (textureChangedConnection)
        QObject::disconnect(textureChangedConnection);

    if (item) {
        QQuickItemPrivate *sd = QQuickItemPrivate::get(item);
        sd->addItemChangeListener(this, QQuickItemPrivate::Geometry);
    }
}

void DQuickItemViewportPrivate::itemGeometryChanged(QQuickItem *item, QQuickGeometryChange data, const QRectF &)
{
    D_Q(DQuickItemViewport);
    // 当DQuickItemViewport或其sourceItem的大小改变时应当更新sourceSizeRatio和maskOffset
    if (data.sizeChange()) {
        markDirtys(DirtySourceSizeRatio | DirtyMaskOffset);

        if (item == q_func())
            markDirty(DirtyMaskSizeRatio);
        q->update();
    }

    // 当DQuickItemViewport的位置改变时应当更新maskOffset
    if (data.positionChange() && item == q_func()) {
        markDirty(DirtyMaskOffset);
        q->update();
    }
}

/*!
 * \~chinese \class DQuickItemViewport
 * \~chinese \brief DQuickItemViewport 类是根据 \a sourceItem 属性设定的 \a QQuickItem
 * \~chinese 作为绘制时的材质来源，这个行为依赖于 \a QQuickItem::textureProvider 提供组件的
 * \~chinese 材质。故可将qml的 Image 和 ShaderEffectSource 作为 sourceItem 使用。
 * \~chinese
 * \~chinese 绘制材质的起始位置为：DQuickItemViewport 自身的位置 + \a sourceOffset 属性的值。
 * \~chinese 一般需将 \a sourceOffset 设置为 \a sourceItem 和 DQuickItemViewport 坐标系的
 * \~chinese 差值，当两者为兄弟关系时此值应该为 "0,0"，否则应该为 DQuickItemViewport 父组件相对
 * \~chinese 于 \a sourceItem 的位置。绘制材质的大小为 DQuickItemViewport 自身的组件大小。
 * \~chinese
 * \~chinese 当 \a radius 属性为 0 时，将直接使用 \a QSGImageNode 作为渲染节点，当其值大于 0
 * \~chinese 时，将使用 QPainter 渲染按 radius 的值绘制一张圆角 QImage 资源，并将其作为 mask
 * \~chinese 材质跟 \a sourceItem 的材质一起运算，并且会使用自定义的片段着色器为材质实现圆角效果。
 * \~chinese \sa QQuickItem::textureProvider
 * \~chinese \sa QQuickItem::isTextureProvider
 */
DQuickItemViewport::DQuickItemViewport(QQuickItem *parent)
    : QQuickItem (parent)
    , DCORE_NAMESPACE::DObject(*new DQuickItemViewportPrivate(this))
{
    QQuickItemPrivate::get(this)->addItemChangeListener(d_func(), DQuickItemViewportPrivate::changeType);
    setFlag(ItemHasContents);
}

DQuickItemViewport::~DQuickItemViewport()
{
    QQuickItemPrivate::get(this)->removeItemChangeListener(d_func(), DQuickItemViewportPrivate::changeType);
}

QQuickItem *DQuickItemViewport::sourceItem() const
{
    D_DC(DQuickItemViewport);
    return d->sourceItem;
}

QPointF DQuickItemViewport::sourceOffset() const
{
    D_DC(DQuickItemViewport);
    return d->sourceOffset;
}

float DQuickItemViewport::radius() const
{
    D_DC(DQuickItemViewport);
    return d->radius;
}

QColor DQuickItemViewport::foregroundColor() const
{
    D_DC(DQuickItemViewport);
    return d->foregroundColor;
}

void DQuickItemViewport::setSourceItem(QQuickItem *sourceItem)
{
    D_D(DQuickItemViewport);

    if (sourceItem && !sourceItem->isTextureProvider()) {
        qWarning() << "DQuickItemViewport: sourceItem is missing or not a texture provider";
        return;
    }

    if (d->sourceItem == sourceItem)
        return;

    if (isComponentComplete()) {
        d->initSourceItem(d->sourceItem, sourceItem);
    }

    d->sourceItem = sourceItem;
    d->markDirtys(DQuickItemViewportPrivate::DirtySourceTexture
                  | DQuickItemViewportPrivate::DirtySourceSizeRatio
                  | DQuickItemViewportPrivate::DirtyMaskOffset);
    Q_EMIT sourceItemChanged(d->sourceItem);
    update();
}

void DQuickItemViewport::setSourceOffset(QPointF sourceOffset)
{
    D_D(DQuickItemViewport);
    if (d->sourceOffset == sourceOffset)
        return;

    d->sourceOffset = sourceOffset;
    d->markDirty(DQuickItemViewportPrivate::DirtyMaskOffset);
    Q_EMIT sourceOffsetChanged(d->sourceOffset);
    update();
}

void DQuickItemViewport::setRadius(float radius)
{
    // TODO：目前深浅色融合在着色器中完成，融合不支持圆角为0
    D_D(DQuickItemViewport);
    if (qFuzzyCompare(d->radius, radius))
        return;

    d->radius = radius;
    d->markDirtys(DQuickItemViewportPrivate::DirtyMaskSizeRatio
                  | DQuickItemViewportPrivate::DirtyMaskTexture);
    Q_EMIT radiusChanged(d->radius);
    update();
}

void DQuickItemViewport::setForegroundColor(const QColor &color)
{
    D_D(DQuickItemViewport);
    if (d->foregroundColor == color)
        return;

    d->foregroundColor = color;
    Q_EMIT foregroundColorChanged(color);
    update();
}

void DQuickItemViewport::itemChange(QQuickItem::ItemChange change, const QQuickItem::ItemChangeData &data)
{
    D_D(DQuickItemViewport);

    switch (static_cast<int>(change)) {
    case ItemAntialiasingHasChanged:
        d->markDirty(DQuickItemViewportPrivate::DirtyMaskTexture);
        update();
        break;
    case ItemDevicePixelRatioHasChanged:
        d->markDirtys(DQuickItemViewportPrivate::DirtyMaskTexture
                      | DQuickItemViewportPrivate::DirtyMaskSizeRatio);
        update();
        break;
    }

    QQuickItem::itemChange(change, data);
}

QSGNode *DQuickItemViewport::updatePaintNode(QSGNode *old, QQuickItem::UpdatePaintNodeData *)
{
    D_D(DQuickItemViewport);
    MaskEffectNode *node = static_cast<MaskEffectNode *>(old);
    if (!d->sourceItem || !d->sourceItem->textureProvider()->texture()) {
        delete old;
        return nullptr;
    }

    // 计算sourceItem应该被绘制的区域，如果此区域大小为0, 则没有必要再继续绘制
    const QPointF &sourceOffset = position() + d->sourceOffset;
    const QRectF &targetRect = QRectF(QPointF(0, 0), size()) & QRectF(-sourceOffset, d->sourceItem->size());
    if (!targetRect.isValid()) {
        delete old;
        return nullptr;
    }

    if (Q_UNLIKELY(!node)) {
        // 创建image node
        if (d->radius > 0) {
            QSGRendererInterface *ri = window()->rendererInterface();
            if (Q_UNLIKELY(ri && ri->graphicsApi() == QSGRendererInterface::Software)) {
                // TODO(zccrs): 软件绘制时暂未支持radius属性
                qWarning() << "Currently does not support software simulation drawing";
            } else {
                node = new MaskEffectNode;
                node->setMaskTexture(d->textureForRadiusMask());
                d->markDirty(DQuickItemViewportPrivate::DirtySourceTexture);
                node->material()->setFlag(QSGMaterial::Blending);
                node->opaqueMaterial()->setFlag(QSGMaterial::Blending);
            }
        } else {
            node->material()->setFlag(QSGMaterial::Blending, false);
            node->opaqueMaterial()->setFlag(QSGMaterial::Blending, false);
        }
    } else {
        // TODO(xiaoyaobing):暂时先不处理无圆角矩形，后面再增加
    }

    // 判断是否应该需要更新材质
    if (d->dirtyState.testFlag(DQuickItemViewportPrivate::DirtySourceTexture)) {
        auto provider = d->sourceItem->textureProvider();
        d->markDirty(DQuickItemViewportPrivate::DirtySourceTexture, false);
        node->setTexture(provider->texture());
        node->setOwnsTexture(false);

        if (!d->textureChangedConnection) {
            // 注意不要将此代码移动到别处，有些对象不允许在非渲染线程中获取 textureProvider
            auto onTextureChanged = [this, d] {
                d->markDirty(DQuickItemViewportPrivate::DirtySourceTexture);
                update();
            };

            d->textureChangedConnection = QObject::connect(provider,
                                                           &QSGTextureProvider::textureChanged,
                                                           this, onTextureChanged);
        }
    }

    const QSizeF &textureSize = node->texture()->textureSize();
    qreal xScale = textureSize.width() / d->sourceItem->width();
    qreal yScale = textureSize.height() / d->sourceItem->height();

    node->setFiltering(smooth() ? QSGTexture::Linear : QSGTexture::Nearest);
    // 更新 DQuickItemViewport 所对应的sourceItem的材质区域
    node->setSourceRect(QRectF((targetRect.x() + sourceOffset.x()) * xScale,
                               (targetRect.y() + sourceOffset.y()) * yScale,
                               targetRect.width() * xScale, targetRect.height() * yScale));
    // 目标绘制区域
    node->setRect(targetRect);
    node->setMaskScale(d->getMaskSizeRatio());
    node->setMaskOffset(d->getMaskOffset());
    node->setSourceScale(d->getSoureSizeRatio());
    node->setForegroundColor(d->foregroundColor);

    return node;
}

void DQuickItemViewport::componentComplete()
{
    D_D(DQuickItemViewport);

    if (d->sourceItem)
        d->initSourceItem(nullptr, d->sourceItem);

    return QQuickItem::componentComplete();
}

DQUICK_END_NAMESPACE
