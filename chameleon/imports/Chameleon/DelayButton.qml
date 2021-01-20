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
import "PixelMetric.js" as PM

T.DelayButton {
    id: control

    implicitWidth: Math.max(background ? background.implicitWidth : 0,
                            contentItem.implicitWidth + leftPadding + rightPadding)
    implicitHeight: Math.max(background ? background.implicitHeight : 0,
                             Math.max(contentItem.implicitHeight,
                                      indicator ? indicator.implicitHeight : 0) + topPadding + bottomPadding)

    padding: PM.ControlPadding
    spacing: PM.ControlSpacing

    transition: Transition {
        NumberAnimation {
            duration: control.delay * (control.pressed ? 1.0 - control.progress : 0.3 * control.progress)
        }
    }

    contentItem: ItemGroup {
        ClippedText {
            clip: control.progress > 0
            clipX: -control.leftPadding + control.progress * control.width
            clipWidth: (1.0 - control.progress) * control.width
            visible: control.progress < 1

            text: control.text
            font: control.font
            color: control.palette.buttonText
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            elide: Text.ElideRight
        }

        ClippedText {
            clip: control.progress > 0
            clipX: -control.leftPadding
            clipWidth: control.progress * control.width
            visible: control.progress > 0

            text: control.text
            font: control.font
            color: control.palette.highlightedText
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            elide: Text.ElideRight
        }
    }

    background: Rectangle {
        implicitWidth: control.text.length ? PM.Button_MiniSize + (4 * PM.ControlRadius) : PM.Button_MiniSize + (2 * PM.ControlRadius)
        implicitHeight: PM.Button_MiniSize
        radius: PM.ControlRadius
        color: control.palette.button

        DRectangleBorder {
            visible: control.visualFocus
            border.color: control.palette.highlight
        }

        Item {
            width: control.progress * parent.width
            height: parent.height
            clip: true

            Rectangle {
                width: parent.parent.width
                height: parent.parent.height
                radius: PM.ControlRadius
                color: control.palette.highlight
            }
        }
    }
}
