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
import QtQuick.Controls 2.4
import "PixelMetric.js" as PM

ToolTip {
    id: control

    property Item target
    property bool connectorVisible: true

    x: 0
    y: target.height + PM.ControlSpacing
    topPadding: PM.AlertControl_VerticalPadding
    bottomPadding: PM.AlertControl_VerticalPadding
    leftPadding: PM.AlertControl_HorizontalPadding
    rightPadding: PM.AlertControl_HorizontalPadding
    implicitWidth: Math.max(background ? background.implicitWidth : 0,
                            contentItem.implicitWidth + leftPadding + rightPadding)
    implicitHeight: Math.max(background ? background.implicitHeight : 0,
                             contentItem.implicitHeight + topPadding + bottomPadding)
    margins: 0
    closePolicy: Popup.NoAutoClose

    background: Item {
        Rectangle {
            id: _background
            anchors.fill: parent
            color: palette.toolTipBase
            border.color: palette.frameBorder
            radius: PM.ControlRadius

        }

        DropShadow {
            z: -1
            anchors.fill: _background
            glowRadius: 10
            offsetY: 6
            color: Qt.rgba(0, 0, 0, 0.25)
            cornerRadius: _background.radius
        }
    }

    contentItem: Text {
        horizontalAlignment: Text.AlignLeft
        verticalAlignment: Text.AlignVCenter
        text: control.text
        color: palette.textWarning
    }

    Rectangle {
        id: connector
        y: - height * (0.75) - control.topMargin - control.topPadding
        width: PM.AlertConnector_Width
        visible: connectorVisible
        height: PM.AlertConnector_Height
        color: palette.base
        border.color: Qt.rgba(0, 0, 0, 0.1)
        border.width: 1
    }

    DropShadow {
        z: -1
        anchors.fill: connector
        color: Qt.rgba(0, 0, 0, 0.1)
        offsetY: 2
        glowRadius: 4
        visible: connector.visible
    }
}
