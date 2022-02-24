/*
 * Copyright (C) 2021 ~ 2022 UnionTech Technology Co., Ltd.
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
import org.deepin.dtk.impl 1.0 as D
import org.deepin.dtk.style 1.0 as DS

Item {
    id: control
    property color color: "transparent"
    property real borderWidth: DS.Style.control.focusBorderWidth
    property real radius: 0

    BoxShadow {
        anchors.fill: _border
        shadowColor: D.DTK.makeColor(parent.color).opacity(-50).color()
        cornerRadius: _border.radius
        shadowBlur: 4
        hollow: true
    }

    Rectangle {
        id: _border

        readonly property real paddings: borderWidth + DS.Style.control.focusBorderPaddings

        anchors {
            fill: parent
            margins: -paddings
        }

        radius: parent.radius + paddings
        border { width: borderWidth; color: parent.color }
        color: "transparent"
    }
}
