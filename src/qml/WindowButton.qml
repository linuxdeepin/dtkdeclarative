/*
 * Copyright (C) 2021 ~ 2022 UnionTech Technology Co., Ltd.
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
    property alias icon: iconLoader
    property alias pressed: mouseArea.pressed
    signal clicked
    property D.Palette textColor: DS.Style.buttonText

    palette.windowText: D.ColorSelector.textColor
    hoverEnabled: true
    contentItem: D.DciIcon {
        id: iconLoader
        palette: D.DTK.makeIconPalette(control.palette)
        sourceSize {
            width: DS.Style.titleBar.buttonWidth
            height: DS.Style.titleBar.buttonHeight
        }
        mode: control.D.ColorSelector.controlState
        theme: control.D.ColorSelector.controlTheme
    }
    MouseArea {
        id: mouseArea
        anchors.fill: parent
        Component.onCompleted: clicked.connect(control.clicked)
    }
    background: Rectangle {
        property D.Palette color1: DS.Style.windowButton.background
        implicitWidth: DS.Style.windowButton.width
        implicitHeight: DS.Style.titleBar.height
        color: D.ColorSelector.color1
    }
}
