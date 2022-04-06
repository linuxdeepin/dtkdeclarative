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

Item {
    id: panel

    property D.Palette backgroundColor: DS.Style.highlightPanel.background
    property D.Palette outerShadowColor: DS.Style.highlightPanel.dropShadow
    property D.Palette innerShadowColor: DS.Style.highlightPanel.innerShadow

    implicitWidth: DS.Style.highlightPanel.width
    implicitHeight: DS.Style.highlightPanel.height
    BoxShadow {
        anchors.fill: backgroundRect
        shadowColor: panel.D.ColorSelector.outerShadowColor
        shadowOffsetY: 4
        shadowBlur: 6
        cornerRadius: backgroundRect.radius
    }

    Rectangle {
        id: backgroundRect
        anchors.fill: parent
        color: panel.D.ColorSelector.backgroundColor
        radius: DS.Style.control.radius
    }

    BoxInsetShadow {
        anchors.fill: backgroundRect
        shadowColor: panel.D.ColorSelector.innerShadowColor
        shadowOffsetY: -1
        shadowBlur: 2
        spread: 1
        cornerRadius: backgroundRect.radius
    }
}
