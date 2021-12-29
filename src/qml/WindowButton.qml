/*
 * Copyright (C) 2021 UnionTech Technology Co., Ltd.
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
import QtQuick.Controls 2.4
import org.deepin.dtk.impl 1.0 as D
import org.deepin.dtk.style 1.0 as DS
import "PixelMetric.js" as PM

Control {
    id: control
    property alias icon: iconLoader
    signal clicked

    property alias pressed: mouseArea.pressed

    property D.Palette highlightedPalette: D.Palette {
        objectName: "buttonText"
        normal: Qt.rgba(0, 0, 0, 0.29)
        normalDark: Qt.rgba(0, 0, 0, 0.29)
        pressed: control.palette.highlight
    }

    D.ColorSelector.palettes: [
        highlightedPalette,
        DS.Style.windowButton
    ]
    hoverEnabled: true
    contentItem: D.DciIcon {
        id: iconLoader
        color: control.D.ColorSelector.buttonText
        sourceSize.width: PM.TitleBar_Height
        sourceSize.height: PM.TitleBar_Height
    }
    MouseArea {
        id: mouseArea
        anchors.fill: parent
        Component.onCompleted: clicked.connect(control.clicked)
    }
    background: Rectangle {
        anchors.fill: parent
        color: control.D.ColorSelector.button
    }
}
