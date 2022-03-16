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

import QtQuick 2.11
import QtQuick.Window 2.11
import org.deepin.dtk.impl 1.0 as D
import org.deepin.dtk.style 1.0 as DS

Window {
    id: __window

    property alias roundJoinRadius: arrowShape.roundJoinRadius
    property alias arrowWidth: arrowShape.arrowWidth
    property alias arrowHeight: arrowShape.arrowHeight
    property alias arrowX: arrowShape.arrowX
    property alias arrowY: arrowShape.arrowY
    property alias arrowDirection: arrowShape.arrowDirection

    flags: Qt.Popup
    D.DWindow.enabled: true
    D.DWindow.clipPath: loader.item
    D.DWindow.enableBlurWindow: true
    D.DWindow.alphaBufferSize: D.DTK.hasBlurWindow ? 8 : 0

    ArrowShapeContainer {
        id: arrowShape
        anchors.fill: parent
        color: "transparent"
    }
}
