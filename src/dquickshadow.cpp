/*
 * Copyright (C) 2021 UnionTech Technology Co., Ltd.
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

#include "dquickshadow.h"
#include "private/dquickshadow_p.h"
#include "private/dshadownode.h"

DQUICK_BEGIN_NAMESPACE

DQuickShadow::DQuickShadow(QQuickItem *parent)
    : QQuickItem(*(new DQuickShadowPrivate), parent)
{
    setFlag(ItemHasContents, true);
}

qreal DQuickShadow::shadowRadius() const
{
    Q_D(const DQuickShadow);

    return d->shadowRadius;
}

void DQuickShadow::setShadowRadius(qreal shadowRadius)
{
    Q_D(DQuickShadow);

    if(qFuzzyCompare(d->shadowRadius, shadowRadius))
        return;

    d->shadowRadius = shadowRadius;
    update();
    Q_EMIT shadowRadiusChanged();
}

QColor DQuickShadow::shadowColor() const
{
    Q_D(const DQuickShadow);

    return d->shadowColor;
}

void DQuickShadow::setShadowColor(const QColor &color)
{
    Q_D(DQuickShadow);

    if (d->shadowColor == color)
        return;

    d->shadowColor = color;
    update();
    Q_EMIT shadowColorChanged();
}

qreal DQuickShadow::spread() const
{
    Q_D(const DQuickShadow);

    return d->spread;
}

void DQuickShadow::setSpread(qreal radius)
{
    Q_D(DQuickShadow);

    if (qFuzzyCompare(d->spread, radius))
        return;

    d->spread = radius;
    update();
    Q_EMIT spreadChanged();
}

qreal DQuickShadow::relativeSizeX() const
{
    Q_D(const DQuickShadow);

    return d->relativeSizeX;
}

void DQuickShadow::setRelativeSizeX(qreal x)
{
    Q_D(DQuickShadow);

    if (qFuzzyCompare(d->relativeSizeX, x))
        return;

    d->relativeSizeX = x;
    update();
    Q_EMIT relativeSizeXChanged();
}

qreal DQuickShadow::relativeSizeY() const
{
    Q_D(const DQuickShadow);

    return d->relativeSizeY;
}

void DQuickShadow::setRelativeSizeY(qreal y)
{
    Q_D(DQuickShadow);

    if (qFuzzyCompare(d->relativeSizeY, y))
        return;

    d->relativeSizeY = y;
    update();
    Q_EMIT relativeSizeYChangd();
}

bool DQuickShadow::fill()
{
    Q_D(const DQuickShadow);

    return d->fill;
}

void DQuickShadow::setFill(bool isFill)
{
    Q_D(DQuickShadow);

    if (d->fill == isFill)
        return;

    d->fill = isFill;
    update();
    Q_EMIT fillChanged();
}

QSGNode *DQuickShadow::updatePaintNode(QSGNode *oldNode, QQuickItem::UpdatePaintNodeData *data)
{
    Q_UNUSED(data);
    Q_D(const DQuickShadow);

    if (width() <= 0 || height() <= 0 || d->shadowColor.alpha() == 0) {
        delete oldNode;
        return nullptr;
    }

    ShadowNode *shadow = static_cast<ShadowNode *>(oldNode);
    if (!shadow) {
        QSGRendererInterface *ri = window()->rendererInterface();
        if (Q_UNLIKELY(ri && ri->graphicsApi() == QSGRendererInterface::Software)) {
            // TODO(xiaoyaobing): Software rendering is not supported yet
            return nullptr;
        } else {
            shadow = new ShadowNode;
        }
    }

    shadow->setRect(QRectF(0, 0, width(), height()));
    shadow->setColor(d->shadowColor);
    shadow->setShadowRadius(d->shadowRadius);
    shadow->setRelativeSizeX(d->relativeSizeX);
    shadow->setRelativeSizeY(d->relativeSizeY);
    shadow->setSpread(d->spread);
    shadow->setFill(d->fill);
    shadow->update();

    return shadow;
}

DQuickShadow::DQuickShadow(DQuickShadowPrivate &dd, QQuickItem *parent)
    : QQuickItem (dd, parent)
{

}

DQUICK_END_NAMESPACE
