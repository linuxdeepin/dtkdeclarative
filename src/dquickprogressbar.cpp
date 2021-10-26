/*
 * Copyright (C) 2020 ~ 2021 Uniontech Software Technology Co.,Ltd.
 *
 * Author:     xiaoyaobing <xiaoyaobing@uniontech.com>
 *
 * Maintainer: xiaoyaobing <xiaoyaobing@uniontech.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "dquickprogressbar.h"
#include "private/dquickprogressbar_p.h"
#include "scenegraph/qsgprogressbarnode.h"

/**
 * @brief DQuickProgressBar::DQuickProgressBar
 *     DProgressBar {
 *       width: 200
 *       height: 200
 *       color: Qt.rgba(1, 0, 0, 1)
 *       radius: 20
 *       value: slider.value
 *       orientation: Qt.Vertical
 *   }
 * @param parent
 */
DQuickProgressBar::DQuickProgressBar(QQuickItem *parent)
    : QQuickItem(*(new DQuickProgressBarPrivate), parent)
{
    setFlag(ItemHasContents, true);
}

QColor DQuickProgressBar::color() const
{
    Q_D(const DQuickProgressBar);

    return d->color;
}

qreal DQuickProgressBar::radius() const
{
    Q_D(const DQuickProgressBar);

    return d->radius;
}

qreal DQuickProgressBar::value() const
{
    Q_D(const DQuickProgressBar);

    return d->value;
}

Qt::Orientation DQuickProgressBar::orientation() const
{
    Q_D(const DQuickProgressBar);

    return d->orientation;
}

/**
 * @brief DQuickProgressBar::setRadius This property holds the corner radius used to draw a rounded rectangle.
 * If radius is non-zero, the rectangle will be painted as a rounded rectangle, otherwise it will be painted as a normal rectangle.
 * The same radius is used by all 4 corners; there is currently no way to specify different radii for different corners.
 * range [0, ∞),However, when displaying, the maximum fillet radius can only be half the height or half the width
 * @param radius
 */
void DQuickProgressBar::setRadius(qreal radius)
{
    Q_D(DQuickProgressBar);

    if (qFuzzyCompare(d->radius, radius))
        return;

    d->radius = radius;
    d->setImplicitAntialiasing(radius != 0.0);
    Q_EMIT radiusChanged(radius);
    update();
}

/**
 * @brief DQuickProgressBar::setColor This property holds the color used to fill the rectangle.
 * The default color is white.
 * @param color
 */
void DQuickProgressBar::setColor(const QColor &color)
{
    Q_D(DQuickProgressBar);

    d->color = color;
    Q_EMIT colorChanged(color);
}

/**
 * @brief DQuickProgressBar::setValue This property holds the value in the range from - to. The default value is 0.0.
 * range [0, 1]
 * @param value
 */
void DQuickProgressBar::setValue(qreal value)
{
    Q_D(DQuickProgressBar);

    if (qFuzzyCompare(d->value, 1.0 - value))
        return;

    d->value = 1.0 - value;
    Q_EMIT valueChanged(d->value);
    update();
}

/**
 * @brief DQuickProgressBar::setOrientation This property holds the orientation.
 * Possible values:
 * Qt.Horizontal
 * Qt.Vertical (default)
 * @param orientation
 */
void DQuickProgressBar::setOrientation(Qt::Orientation orientation)
{
    Q_D(DQuickProgressBar);

    if (d->orientation == orientation)
        return;

    d->orientation = orientation;
    update();
}

QSGNode *DQuickProgressBar::updatePaintNode(QSGNode *oldNode, QQuickItem::UpdatePaintNodeData *data)
{
    Q_D(const DQuickProgressBar);

    QSGProgressBarNode *node = static_cast<QSGProgressBarNode *>(oldNode);

    if (!node) {
        node = new QSGProgressBarNode(d->orientation);
        node->setColor(d->color);
        node->setValue(d->value);
        node->setRect(QRectF(0, 0, width(), height()));
        node->setRadius(d->radius);
        node->updateGeometry();
        return node;
    }

    node->setColor(d->color);
    node->setValue(d->value);
    node->setRadius(d->radius);

    if (node->geometryIsDirty())
        node->updateGeometry();

    return node;
}

DQuickProgressBar::DQuickProgressBar(DQuickProgressBarPrivate &dd, QQuickItem *parent)
    :QQuickItem(dd, parent)
{

}
