/*
 * Copyright (C) 2021 ~ 2022 UnionTech Technology Co., Ltd.
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

T.Button {
    id: control

    property D.Palette color1: checked ? checkedColor1 : (highlighted ? DS.Style.highlightedButton1 : DS.Style.button1)
    property D.Palette color2: highlighted ? DS.Style.highlightedButton2 : DS.Style.button2
    property D.Palette textColor: checked ? checkedTextColor : (highlighted ? DS.Style.highlightedButtonText : DS.Style.buttonText)
    property D.Palette borderColor: highlighted ? DS.Style.highlightedButtonBorder : DS.Style.buttonBorder
    property D.Palette dropShadowColor: highlighted ? DS.Style.highlightedButtonDropShadow : DS.Style.buttonDropShadow
    property D.Palette innerShadowColor1: highlighted ? DS.Style.highlightedButtonInnerShadow1 : DS.Style.buttonInnerShadow1
    property D.Palette innerShadowColor2: highlighted ? DS.Style.highlightedButtonInnerShadow2 : DS.Style.buttonInnerShadow2

    D.Palette {
        id: checkedColor1
        normal: control.palette.highlight
        hovered: D.DTK.adjustColor(control.palette.highlight, 0, 0, +10, 0, 0, 0, 0)
        pressed: D.DTK.adjustColor(control.palette.highlight, 0, 0, -10, 0, 0, 0, 0)
    }

    D.Palette {
        id: checkedTextColor
        normal: control.palette.highlightedText
        hovered: D.DTK.adjustColor(control.palette.highlightedText, 0, 0, +10, 0, 0, 0, 0)
        pressed: D.DTK.adjustColor(control.palette.highlightedText, 0, 0, -20, 0, 0, 0, 0)
    }

    implicitWidth: DS.Style.control.implicitWidth(control)
    implicitHeight: DS.Style.control.implicitHeight(control)

    topPadding: DS.Style.control.vPadding
    bottomPadding: DS.Style.control.vPadding
    spacing: DS.Style.control.spacing
    opacity: D.ColorSelector.controlState === D.DTK.DisabledState ? 0.4 : 1
    D.DciIcon.mode: D.ColorSelector.controlState
    icon {
        width: DS.Style.button.iconSize
        height: DS.Style.button.iconSize
        color: D.ColorSelector.textColor
    }

    background: Item {
        id: backgroundItem
        implicitWidth: DS.Style.button.width
        implicitHeight: DS.Style.button.height
        visible: !control.flat || control.down || control.checked || control.highlighted || control.visualFocus || control.hovered

        BoxShadow {
            anchors.fill: backgroundRect
            shadowBlur: 6
            shadowOffsetY: 4
            shadowColor: control.D.ColorSelector.dropShadowColor
            cornerRadius: backgroundRect.radius
            visible: control.D.ColorSelector.family === D.Palette.CommonColor
        }

        Rectangle {
            id: backgroundRect
            objectName: "ColorSelectorMaster"
            D.ColorSelector.hovered: false

            Gradient {
                id: backgroundGradient
                // Use the backgroundItem's colorselecor can filter the hovered state.
                GradientStop { position: 0.0; color: backgroundRect.D.ColorSelector.color1}
                GradientStop { position: 0.96; color: backgroundRect.D.ColorSelector.color2}
            }

            anchors.fill: parent
            radius: DS.Style.control.radius
            gradient: D.ColorSelector.color1 === D.ColorSelector.color2 ? null : backgroundGradient
            color: D.ColorSelector.color1
        }

        Gradient {
            id: hoverBackgroundGradient
            GradientStop { position: 0.0; color: control.D.ColorSelector.color1 }
            GradientStop { position: 0.96; color: control.D.ColorSelector.color2 }
        }

        CicleSpreadAnimation {
            id: hoverAnimation
            anchors.fill: backgroundRect
            visible: control.D.ColorSelector.controlState === D.DTK.HoveredState
                     && control.D.ColorSelector.family === D.Palette.CommonColor

            Rectangle {
                anchors.fill: parent
                radius: backgroundRect.radius
                gradient: control.D.ColorSelector.color1 === control.D.ColorSelector.color2 ? null : hoverBackgroundGradient
                color: control.D.ColorSelector.color1
            }
        }

        BoxShadow {
            inner: true
            anchors.fill: backgroundRect
            shadowBlur: 2
            shadowOffsetY: innerShadow2.shadowColor !== "transparent" ? -3 : -1
            spread: 1
            shadowColor: control.D.ColorSelector.innerShadowColor1
            cornerRadius: backgroundRect.radius
            visible: control.D.ColorSelector.controlState !== D.DTK.PressedState
                     && control.D.ColorSelector.family === D.Palette.CommonColor
        }

        BoxShadow {
            id: innerShadow2
            inner: true
            anchors.fill: backgroundRect
            shadowBlur: 1
            shadowOffsetY: 2
            shadowColor: control.D.ColorSelector.innerShadowColor2
            cornerRadius: backgroundRect.radius
            visible: control.D.ColorSelector.family === D.Palette.CommonColor
        }

        Rectangle {
            anchors.fill: backgroundRect
            color: "transparent"
            radius: backgroundRect.radius
            border {
                width: DS.Style.control.borderWidth
                color: control.D.ColorSelector.borderColor
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
        color: control.D.ColorSelector.textColor
        icon: D.DTK.makeIcon(control.icon, control.D.DciIcon)
    }
}
