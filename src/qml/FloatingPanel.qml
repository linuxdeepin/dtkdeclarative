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

    padding: DS.Style.floatingMessage.panel.radius / 2

    property D.Palette backgroundColor: DS.Style.floatingMessage.panel.background
    property D.Palette dropShadowColor: DS.Style.floatingMessage.panel.dropShadow
    property D.Palette borderColor: DS.Style.control.border
    property int radius: DS.Style.floatingMessage.panel.radius

    background: D.InWindowBlur {
        implicitWidth: DS.Style.floatingMessage.panel.width
        implicitHeight: DS.Style.floatingMessage.panel.height
        radius: control.radius
        offscreen: true

        D.ItemViewport {
            anchors.fill: parent
            fixed: true
            sourceItem: parent
            radius: control.radius
            hideSource: false
        }

        BoxShadow {
            anchors.fill: backgroundRect
            shadowOffsetX: 0
            shadowOffsetY: 4
            shadowColor: control.D.ColorSelector.dropShadowColor
            shadowBlur: 20
            cornerRadius: backgroundRect.radius
            spread: 0
            hollow: true
        }
        Rectangle {
            id: backgroundRect
            anchors.fill: parent
            radius: control.radius
            color: control.D.ColorSelector.backgroundColor
            border {
                color: control.D.ColorSelector.borderColor
                width: DS.Style.control.borderWidth
            }
        }
    }
}
