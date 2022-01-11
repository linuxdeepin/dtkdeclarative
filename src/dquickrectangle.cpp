/*
 * Copyright (C) 2022 UnionTech Technology Co., Ltd.
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
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "dquickrectangle.h"
#include "private/dquickrectangle_p.h"
#include "private/drectanglenode.h"

DQUICK_BEGIN_NAMESPACE

DQuickRectangle::DQuickRectangle(QQuickItem *parent)
    : QQuickItem (*(new DQuickRectanglePrivate), parent)
{
    setFlag(ItemHasContents);
}

QColor DQuickRectangle::color() const
{
    Q_D(const DQuickRectangle);

    return d->color;
}

void DQuickRectangle::setColor(const QColor &color)
{
    Q_D(DQuickRectangle);

    if (d->color == color)
        return;

    d->color = color;
    update();
    Q_EMIT colorChanged();
}

qreal DQuickRectangle::radius() const
{
    Q_D(const DQuickRectangle);

    return d->radius;
}

void DQuickRectangle::setRadius(qreal radius)
{
    Q_D(DQuickRectangle);

    if(qFuzzyCompare(d->radius, radius))
        return;

    d->radius = radius;
    update();
    Q_EMIT radiusChanged();
}

DQuickRectangle::Corners DQuickRectangle::corners() const
{
    Q_D(const DQuickRectangle);

    return d->corners;
}

void DQuickRectangle::setCorners(Corners corners)
{
    Q_D(DQuickRectangle);

    if (d->corners == corners)
        return;

    d->corners = corners;
    update();
    Q_EMIT cornersChanged();
}

QSGNode *DQuickRectangle::updatePaintNode(QSGNode *oldNode, QQuickItem::UpdatePaintNodeData *data)
{
    Q_UNUSED(data);
    Q_D(DQuickRectangle);

    if (width() <= 0 || height() <= 0 || d->color.alpha() == 0) {
        delete oldNode;
        return nullptr;
    }

    QSGRendererInterface *ri = window()->rendererInterface();
    if (Q_UNLIKELY(ri && ri->graphicsApi() == QSGRendererInterface::Software)) {
        DSoftRectangleNode *rectangle = static_cast<DSoftRectangleNode *>(oldNode);
        if (!rectangle) {
            rectangle = new DSoftRectangleNode(this);
        }

        rectangle->setColor(d->color);
        rectangle->setRadius(d->radius * d->window->effectiveDevicePixelRatio());
        rectangle->setCorners(corners());
        return rectangle;
    } else {
        DRectangleNode *rectangle = static_cast<DRectangleNode *>(oldNode);
        if (!rectangle) {
            rectangle = new DRectangleNode;
        }

        if (d->radius > 0)
            rectangle->setMakTexture(d->textureForRadiusMask());

        rectangle->setRect(QRectF(0, 0, width(), height()));
        rectangle->setColor(d->color);
        rectangle->setRadius(d->radius);
        rectangle->setCorners(corners());
        rectangle->update();

        return rectangle;
    }
}

DQuickRectangle::DQuickRectangle(DQuickRectanglePrivate &dd, QQuickItem *parent)
    : QQuickItem (dd, parent)
{
}

DQUICK_END_NAMESPACE
