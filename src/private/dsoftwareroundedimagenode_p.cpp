/*
 * Copyright (C) 2021 Uniontech Technology Co., Ltd.
 *
 * Author:     JiDe Zhang <zhangjide@deepin.org>
 *
 * Maintainer: JiDe Zhang <zhangjide@deepin.org>
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
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */
#include "dsoftwareroundedimagenode_p.h"

#if QT_VERSION < QT_VERSION_CHECK(5, 14, 0)
#include <private/qsgtexture_p.h>
#else
#include <private/qsgplaintexture_p.h>
#endif
#include <private/qquickitem_p.h>
#include <QPainter>

DQUICK_BEGIN_NAMESPACE

DSoftwareRoundedImageNode::DSoftwareRoundedImageNode(QQuickItem *owner)
    : item(owner)
{

}

void DSoftwareRoundedImageNode::setTexture(QSGTexture *texture)
{
    m_texture = texture;
    markDirty(DirtyMaterial);
}

void DSoftwareRoundedImageNode::setRadius(qreal radius)
{
    if (qFuzzyCompare(radius, this->radius))
        return;
    this->radius = radius;

    clipPath = QPainterPath();
    if (radius > 0) {
        clipPath.addRoundedRect(targetRect, radius, radius);
    }

    markDirty(DirtyMaterial);
}

void DSoftwareRoundedImageNode::setSmooth(bool smooth)
{
    if (this->smooth == smooth)
        return;
    this->smooth = smooth;
    markDirty(DirtyMaterial);
}

void DSoftwareRoundedImageNode::setSourceRect(const QRectF &source)
{
    if (this->sourceRect == source)
        return;
    this->sourceRect = source;
    markDirty(DirtyMaterial);
}

void DSoftwareRoundedImageNode::setRect(const QRectF &target)
{
    if (this->targetRect == target)
        return;
    this->targetRect = target;

    if (radius > 0) {
        clipPath = QPainterPath();
        clipPath.addRoundedRect(targetRect, radius, radius);
    }

    markDirty(DirtyMaterial);
}

void DSoftwareRoundedImageNode::render(const RenderState *state)
{
    if (!sourceRect.isValid())
        return;

    QSGRendererInterface *rif = item->window()->rendererInterface();
    QPainter *p = static_cast<QPainter *>(rif->getResource(item->window(),
                                                           QSGRendererInterface::PainterResource));
    Q_ASSERT(p);

    const QRegion *clipRegion = state->clipRegion();
    if (clipRegion && !clipRegion->isEmpty())
        p->setClipRegion(*clipRegion, Qt::ReplaceClip); // must be done before setTransform

    p->setTransform(matrix()->toTransform());
    p->setOpacity(inheritedOpacity());
    p->setRenderHint(QPainter::Antialiasing, smooth);

    if (Q_LIKELY(!clipPath.isEmpty())) {
        p->setClipPath(clipPath, Qt::IntersectClip);
    }

    updateCachedImage();
    p->drawImage(targetRect, cachedImage, sourceRect);
}

void DSoftwareRoundedImageNode::releaseResources()
{
    static QImage globalNullImage;
    cachedImage = globalNullImage;
}

QSGRenderNode::RenderingFlags DSoftwareRoundedImageNode::flags() const
{
    RenderingFlags rf = BoundedRectRendering;
    if (Q_UNLIKELY(qIsNull(radius)) && !cachedImage.hasAlphaChannel())
        rf |= OpaqueRendering;
    return rf;
}

QRectF DSoftwareRoundedImageNode::rect() const
{
    return targetRect;
}

void DSoftwareRoundedImageNode::updateCachedImage()
{
    Q_ASSERT(m_texture);
    if (auto image = qobject_cast<QSGPlainTexture*>(m_texture)) {
        cachedImage = image->image();
    } else if (QSGLayer *pt = qobject_cast<QSGLayer *>(m_texture)) {
        cachedImage = pt->toImage();
    } else {
        DSoftwareRoundedImageNode::releaseResources();
    }
}

DQUICK_END_NAMESPACE
