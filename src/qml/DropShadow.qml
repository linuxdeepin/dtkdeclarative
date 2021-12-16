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

import QtQuick 2.0
import org.deepin.dtk 1.0 as D

Item {
    id: rootItem

    /*! This property defines the offset of the shadow in the x-axis direction. */
    property real offsetX: 0.0

    /*! This property defines the offset of the shadow in the y-axis direction. */
    property real offsetY: 0.0

    /*! This property defines how many pixels outside(or inside) the item area are reached by the shadow. */
    property real glowRadius: 0.0

    /*! This property defines how large part of the shadow color is strengthened near the source edges. */
    property real spread: 0.0

    /*! This property defines the the shadow color. */
    property color color: "black"

    /*! This property defines corners size of the control that draws the shadow. */
    property real cornerRadius: glowRadius

    /*! This property defines does the shadow fill the entire area. */
    property alias fill: effct.fill

    D.ShadowEffect {
        id: effct

        property real inverseSpread: 1.0 - rootItem.spread
        property real cornerRadius: {
            var maxCornerRadius = Math.min(rootItem.width, rootItem.height) / 2 + glowRadius;
            return Math.max(0, Math.min(rootItem.cornerRadius + rootItem.glowRadius, maxCornerRadius))
        }

        x: (parent.width - width) / 2.0 + offsetX
        y: (parent.height - height) / 2.0 + offsetY
        width: parent.width + rootItem.glowRadius * 2 + cornerRadius * 2
        height: parent.height + rootItem.glowRadius * 2 + cornerRadius * 2
        shadowColor: rootItem.color
        spread: rootItem.spread / 2.0
        shadowRadius: rootItem.glowRadius * 1 + cornerRadius * 2
        relativeSizeX: ((inverseSpread * inverseSpread) * rootItem.glowRadius + cornerRadius * 2.0) / width
        relativeSizeY: relativeSizeX * (width / height)
    }
}
