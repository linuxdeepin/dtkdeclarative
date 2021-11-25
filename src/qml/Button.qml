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

    D.ColorSelector.palettes: [
        DS.Style.button1,
        DS.Style.button2,
        DS.Style.buttonText
    ]
    D.ColorSelector.hovered: false

    D.ColorSelector {
        id: csForHover
        control: control.D.ColorSelector.control
        palettes: control.D.ColorSelector.palettes
        hovered: true
    }

    implicitWidth: Math.max(background ? background.implicitWidth : 0,
                            contentItem.implicitWidth + leftPadding + rightPadding)
    implicitHeight: Math.max(background ? background.implicitHeight : 0,
                             Math.max(contentItem.implicitHeight, indicator ? indicator.implicitHeight : 0) + topPadding + bottomPadding)

    padding: PM.ControlPadding
    spacing: PM.ControlSpacing
    D.DciIcon.mode: D.ColorSelector.controlState

    icon.width: DS.Style.buttonIconWidth
    icon.height: DS.Style.buttonIconHeight
    icon.color: D.ColorSelector.palettes ? D.ColorSelector.buttonText : null

    background: Item {
        implicitWidth: control.text.length ? PM.Button_MiniSize + (4 * PM.ControlRadius) : PM.Button_MiniSize + (2 * PM.ControlRadius)
        implicitHeight: PM.Button_MiniSize

        RectangularShadow {
            anchors.fill: backgroundRect
            offsetX: 0
            offsetY: 4
            glowRadius: backgroundRect.radius
            color: palette.shadow
        }

        Gradient {
            id: backgroundGradient
            GradientStop { position: 0.0; color: control.D.ColorSelector.button1 }
            GradientStop { position: 0.96; color: control.D.ColorSelector.button2 }
        }

        Rectangle {
            id: backgroundRect

            anchors.fill: parent
            radius: PM.ControlRadius
            gradient: Gradient {
                GradientStop { position: 0.0; color: control.D.ColorSelector.button1 }
                GradientStop { position: 0.96; color: control.D.ColorSelector.button2 }
            }
        }

        CicleSpreadAnimation {
            id: hoverAnimation
            anchors.fill: backgroundRect
            visible: control.D.ColorSelector.controlState === D.DTK.HoveredState

            Rectangle {
                anchors.fill: parent
                radius: backgroundRect.radius
                gradient: Gradient {
                    GradientStop { position: 0.0; color: csForHover.button1 }
                    GradientStop { position: 0.96; color: csForHover.button2 }
                }
            }
        }
    }

    onHoveredChanged: {
        if (!hoverAnimation)
            return

        if (hovered) {
            var pos = D.DTK.cursorPosition()
            hoverAnimation.centerPoint = hoverAnimation.mapFromGlobal(pos.x, pos.y)
            hoverAnimation.start()
        } else {
            hoverAnimation.stop()
        }
    }

    contentItem: D.IconLabel {
        spacing: control.spacing
        mirrored: control.mirrored
        display: control.display
        text: control.text
        font: control.font
        color: control.D.ColorSelector.controlState === D.DTK.HoveredState
               ? csForHover.buttonText
               : control.D.ColorSelector.buttonText
        icon: D.DTK.makeIcon(control.icon, control.D.DciIcon)
    }
}

