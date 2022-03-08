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
import "PixelMetric.js" as PM

T.ProgressBar {
    id: control

    implicitWidth: Math.max(background ? background.implicitWidth : 0,
                            contentItem.implicitWidth + leftPadding + rightPadding)
    implicitHeight: Math.max(background ? background.implicitHeight : 0,
                             contentItem.implicitHeight + topPadding + bottomPadding)

    property color contentColor: control.palette.highlight
    property int valueWidth: position * width

    contentItem: Rectangle {
        color: control.palette.button
        radius: PM.ControlRadius

        Item {
            width: valueWidth
            height: control.height
            clip: true

            Item {
                width: control.width
                height: control.height

                Rectangle {
                    anchors.centerIn: parent
                    width: parent.height
                    height: parent.width
                    radius: PM.ControlRadius
                    rotation: -90
                    gradient: Gradient {
                        GradientStop { position: 0.0; color: contentColor }
                        GradientStop { position: 1.0; color: Qt.rgba(contentColor.r + 0.33, contentColor.g, contentColor.b + 0.44, contentColor.a) }
                    }
                }
            }
        }

        Text {
            height: control.height - (2 * PM.ControlRadius)
            x: PM.ControlRadius
            y: (control.height - height) / 2
            color: valueWidth < PM.ControlRadius ?
                       control.palette.buttonText : control.palette.highlightedText
            text: value.toString() + Qt.locale().percent
        }
    }

    background: Rectangle {
        implicitWidth: PM.ProgressBar_Width
        implicitHeight: PM.ProgressBar_Height
    }
}
