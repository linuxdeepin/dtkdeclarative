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
import QtQuick.Templates 2.4 as T
import org.deepin.dtk.impl 1.0 as D
import org.deepin.dtk.style 1.0 as DS

T.MenuItem {
    id: control

    property bool useIndicatorPadding: menu && menu.existsChecked
    implicitWidth: DS.Style.control.implicitWidth(control)
    implicitHeight: DS.Style.control.implicitHeight(control)
    baselineOffset: contentItem.y + contentItem.baselineOffset
    padding: DS.Style.control.padding
    spacing: DS.Style.control.spacing
    icon {
        height: DS.Style.menu.itemIconSize.height
        width: DS.Style.menu.itemIconSize.height
    }

    property D.Palette itemColor: D.Palette {
        hovered: D.DTK.makeColor(D.Color.HighlightedText)
    }
    palette.windowText: hovered ? D.ColorSelector.itemColor : undefined
    D.DciIcon.mode: D.ColorSelector.controlState
    D.DciIcon.palette: D.DTK.makeIconPalette(palette)
    contentItem: D.IconLabel {
        readonly property real arrowPadding: control.subMenu && control.arrow ? control.arrow.width + control.spacing : 0
        readonly property real indicatorPadding: control.useIndicatorPadding && control.indicator ? control.indicator.width + control.spacing : 0

        leftPadding: !control.mirrored ? indicatorPadding : arrowPadding
        rightPadding: control.mirrored ? indicatorPadding : arrowPadding
        spacing: control.spacing
        mirrored: control.mirrored
        display: control.display
        alignment: Qt.AlignLeft
        text: control.text
        font: control.font
        color: control.palette.windowText
        icon: D.DTK.makeIcon(control.icon, control.D.DciIcon)
    }

    indicator: D.QtIcon {
        anchors {
            left: control.left
            leftMargin: control.mirrored ? control.width - width - control.rightPadding : control.leftPadding
            verticalCenter: parent.verticalCenter
        }
        visible: control.checked
        name: "mark_indicator"
        color: control.D.ColorSelector.itemColor
    }

    arrow: D.DciIcon {
        anchors {
            right: parent.right
            rightMargin: control.mirrored ? control.width - width - control.rightPadding : control.rightPadding
            verticalCenter: parent.verticalCenter
        }
        visible: control.subMenu
        mirror: control.mirrored
        name: control.subMenu ? "menu_arrow" : ""
        palette: control.D.DciIcon.palette
    }

    background: Item {
        implicitWidth: DS.Style.menu.itemWidth
        implicitHeight: DS.Style.menu.itemHeight
        Rectangle {
            anchors.fill: parent
            visible: !control.hovered && control.subMenu && control.subMenu.opened
            color: DS.Style.menu.subMenuOpendBackground
            radius: DS.Style.control.radius
        }

        HighlightPanel {
            anchors.fill: parent
            visible: control.hovered
        }
    }
}
