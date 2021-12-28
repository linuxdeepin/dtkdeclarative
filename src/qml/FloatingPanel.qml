/*
 * Copyright (C) 2022 UnionTech Technology Co., Ltd.
 *
 * Author:     yeshanshan <yeshanshan@uniontech.com>
 *
 * Maintainer: yeshanshan <yeshanshan@uniontech.com>
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

Control {
    id: control

    padding: DS.Style.floatingPanel.radius / 2

    background: BoxShadow {
        anchors.fill: parent
        shadowOffsetX: 0
        shadowOffsetY: 6
        shadowColor: D.DTK.selectColor(control.palette.window, DS.Style.floatingPanel.lightShadowBackground, DS.Style.floatingPanel.darkShadowBackground)
        shadowBlur: 20
        cornerRadius: backgroundRect.radius
        spread: 0
        inner: false
        Rectangle {
            id: backgroundRect
            anchors.fill: parent
            radius: DS.Style.floatingPanel.radius
            color: D.DTK.selectColor(control.palette.window, DS.Style.floatingPanel.lightBackground, DS.Style.floatingPanel.darkBackground)
            border {
                color: D.DTK.selectColor(control.palette.window, DS.Style.control.lightBorder, DS.Style.control.darkBorder)
                width: DS.Style.control.borderWidth
            }
        }
    }
}
