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
import QtGraphicalEffects 1.0

T.Button {
    id: control

    property var paletteGroup: [
        DS.Style.button1,
        DS.Style.button2,
        DS.Style.buttonText,
        DS.Style.buttonBorder
    ]

    property var highlightedPaletteGroup: [
        DS.Style.suggestButton1,
        DS.Style.suggestButton2,
        DS.Style.suggestButtonText,
        DS.Style.suggestButtonBorder
    ]

    D.ColorSelector.palettes: highlighted ? highlightedPaletteGroup : paletteGroup

    implicitWidth: DS.Style.control.implicitWidth(control)
    implicitHeight: DS.Style.control.implicitHeight(control)

    topPadding: DS.Style.control.vPadding
    bottomPadding: DS.Style.control.vPadding
    spacing: DS.Style.control.spacing
    opacity: enabled ? 1 : 0.4
    D.DciIcon.mode: D.ColorSelector.controlState

    icon {
        width: DS.Style.button.iconSize
        height: DS.Style.button.iconSize
        color: D.ColorSelector.palettes ? D.ColorSelector.buttonText : null
    }

    background: Item {
        implicitWidth: DS.Style.button.width
        implicitHeight: DS.Style.button.height
        visible: !control.flat || control.down || control.checked || control.highlighted || control.visualFocus || control.hovered

        Component.onCompleted: {
            paletteGroup.push(checkedPalette)
            control.D.ColorSelector.palettes.push(checkedPalette)
        }

        D.Palette {
            id: checkedPalette
            objectName: "button1"
            enabled: control.checked
            normal: control.palette.highlight
            hovered: D.DTK.adjustColor(control.palette.highlight, 0, 0, +10, 0, 0, 0, 0)
            pressed: D.DTK.adjustColor(control.palette.highlight, 0, 0, -10, 0, 0, 0, 0)
        }

        D.ColorSelector {
            id: csForHover
            control: control.D.ColorSelector.control
            palettes: control.D.ColorSelector.palettes
            hovered: false
        }

        Gradient {
            id :backgroundGradient
            GradientStop { position: 0.0; color: csForHover.palettes ? csForHover.button1 : null }
            GradientStop { position: 0.96; color: csForHover.palettes ? csForHover.button2 : null }
        }

        Rectangle {
            id: backgroundRect

            anchors.fill: parent
            radius: DS.Style.control.radius
            border {
                width: DS.Style.control.borderWidth
                color: control.D.ColorSelector.buttonBorder
            }
            gradient: checkedPalette.enabled ? null : backgroundGradient
            color: control.D.ColorSelector.button1
        }

        Gradient {
            id: hoverBackgroundGradient
            GradientStop { position: 0.0; color: control.D.ColorSelector.button1 }
            GradientStop { position: 0.96; color: control.D.ColorSelector.button2 }
        }

        CicleSpreadAnimation {
            id: hoverAnimation
            anchors.fill: backgroundRect
            visible: control.D.ColorSelector.controlState === D.DTK.HoveredState

            Rectangle {
                anchors.fill: parent
                radius: backgroundRect.radius
                border {
                    width: backgroundRect.border.width
                    color: backgroundRect.border.color
                }
                gradient: checkedPalette.enabled ? null : hoverBackgroundGradient
                color: backgroundRect.color
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
        color: control.D.ColorSelector.buttonText
        icon: D.DTK.makeIcon(control.icon, control.D.DciIcon)

        Component.onCompleted: {
            paletteGroup.push(checkedTextPalette)
            control.D.ColorSelector.palettes.push(checkedTextPalette)
        }

        D.Palette {
            id: checkedTextPalette
            objectName: "buttonText"
            enabled: control.checked
            normal: control.palette.highlightedText
            hovered: D.DTK.adjustColor(control.palette.highlightedText, 0, 0, +10, 0, 0, 0, 0)
            pressed: D.DTK.adjustColor(control.palette.highlightedText, 0, 0, -20, 0, 0, 0, 0)
        }
    }
}

