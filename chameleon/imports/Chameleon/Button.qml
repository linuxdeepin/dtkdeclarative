/*
 * Copyright (C) 2020 ~ 2020 Uniontech Technology Co., Ltd.
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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

import QtQuick 2.11
import QtQuick.Controls 2.4
import QtQuick.Controls.impl 2.4
import QtQuick.Templates 2.4 as T
import org.deepin.dtk 1.0 as D
import "PixelMetric.js" as PM

T.Button {
    id: control

    implicitWidth: Math.max(background ? background.implicitWidth : 0,
                            contentItem.implicitWidth + leftPadding + rightPadding)
    implicitHeight: Math.max(background ? background.implicitHeight : 0,
                             Math.max(contentItem.implicitHeight, indicator ? indicator.implicitHeight : 0) + topPadding + bottomPadding)

    padding: PM.ControlPadding
    spacing: PM.ControlSpacing

    property alias radius:  backgroundRect.radius
    property string iconName: ""
    property color textColor: palette.buttonText
    property color initGradTopColor: palette.light
    property color initGradBottomColor: palette.dark

    background: Item {
        implicitWidth: control.text.length ? PM.Button_MiniSize + (4 * PM.ControlRadius) : PM.Button_MiniSize + (2 * PM.ControlRadius)
        implicitHeight: PM.Button_MiniSize

        D.DropShadow {
            anchors.fill: backgroundRect
            offsetX: 0
            offsetY: 4
            glowRadius: backgroundRect.radius
            color: palette.shadow
        }

        Rectangle {
            id: backgroundRect
            property color gradTopColor: initGradTopColor
            property color gradBottomColor: initGradBottomColor

            anchors.fill: parent
            radius: PM.ControlRadius
            color: control.palette.button
        }

        D.CicleSpreadAnimation {
            id: hoverAnimation
            hoverColor: backgroundRect.gradBottomColor
            source: control
            visible: hoverEnabled
        }
    }

    onHoveredChanged: {
        if (hovered) {
            hoverAnimation.centerPoint = hoverAnimation.mapFromGlobal(D.DTK.cursorPosition())
            hoverAnimation.start()
        } else {
            hoverAnimation.stop()
        }
    }

    contentItem: IconLabel {
        spacing: control.spacing
        mirrored: control.mirrored
        display: control.display

        icon: control.icon
        text: control.text
        font: control.font
        color: textColor
    }
}

