/*
 * Copyright (C) 2021 UnionTech Technology Co., Ltd.
 *
 * Author:     JiDe Zhang <zhangjide@deepin.org>
 *
 * Maintainer: JiDe Zhang <zhangjide@deepin.org>
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
import QtQuick.Controls.impl 2.4
import QtQuick.Templates 2.4 as T
import org.deepin.dtk.impl 1.0 as D
import org.deepin.dtk.style 1.0 as DS
import "PixelMetric.js" as PM

T.Button {
    id: control

    // TODO: Support the attached properies for QQuickControl on ColorSelector
    property D.ColorSelector colorSelector: D.ColorSelector {
        control: control
        palettes: [
            DS.Style.button1,
            DS.Style.button2,
            DS.Style.buttonText
        ]
    }

    implicitWidth: Math.max(background ? background.implicitWidth : 0,
                            contentItem.implicitWidth + leftPadding + rightPadding)
    implicitHeight: Math.max(background ? background.implicitHeight : 0,
                             Math.max(contentItem.implicitHeight, indicator ? indicator.implicitHeight : 0) + topPadding + bottomPadding)

    padding: PM.ControlPadding
    spacing: PM.ControlSpacing

    background: Item {
        implicitWidth: control.text.length ? PM.Button_MiniSize + (4 * PM.ControlRadius) : PM.Button_MiniSize + (2 * PM.ControlRadius)
        implicitHeight: PM.Button_MiniSize

        DropShadow {
            anchors.fill: backgroundRect
            offsetX: 0
            offsetY: 4
            glowRadius: backgroundRect.radius
            color: palette.shadow
        }

        Rectangle {
            id: backgroundRect

            anchors.fill: parent
            radius: PM.ControlRadius
            gradient: Gradient {
                GradientStop { position: 0.0; color: colorSelector.button1 }
                GradientStop { position: 0.96; color: colorSelector.button2 }
            }
        }

        CicleSpreadAnimation {
            id: hoverAnimation
            hoverColor: palette.dark
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
        color: colorSelector.buttonText
    }
}

