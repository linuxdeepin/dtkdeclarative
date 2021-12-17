/*
 * Copyright (C) 2021 UnionTech Technology Co., Ltd.
 *
 * Author:     Chen Bin <chenbin@uniontech.com>
 *
 * Maintainer: Chen Bin <chenbin@uniontech.com>
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
import org.deepin.dtk.style 1.0 as DS
import "PixelMetric.js" as PM

Item {
    property color borderColor: "transparent"

    anchors.centerIn: parent
    width: parent.width + 2 * DS.Style.control.focusBorderWidth
    height: parent.height + 2 * DS.Style.control.focusBorderWidth

    Rectangle {
        id: _border
        anchors.fill: parent
        radius: DS.Style.control.radius + DS.Style.control.focusBorderWidth
        border { width: DS.Style.control.focusBorderWidth; color: borderColor }
        color: "transparent"
    }

    // TODO(Chen Bin): Replace it to Dtk DropShadow
//    RectangularShadow {
//        anchors.fill: _border
//        color: borderColor
//        glowRadius: 4
//        fill: true
//    }
}
