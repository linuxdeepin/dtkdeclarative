/*
 * Copyright (C) 2022 UnionTech Technology Co., Ltd.
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
import org.deepin.dtk.impl 1.0 as D
import org.deepin.dtk.style 1.0 as DS

Item {
    id: control
    property Item button
    property int radius: DS.Style.control.radius
    property D.Palette color1: selectValue(DS.Style.button1, DS.Style.checkedButton, DS.Style.highlightedButton1)
    property D.Palette color2: selectValue(DS.Style.button2, DS.Style.checkedButton, DS.Style.highlightedButton2)
    property D.Palette insideBorderColor: selectValue(DS.Style.buttonInsideBorder, null, DS.Style.highlightedButtonBorder)
    property D.Palette outsideBorderColor: selectValue(DS.Style.buttonOutsideBorder, null, null)
    property D.Palette dropShadowColor: selectValue(DS.Style.buttonDropShadow, DS.Style.checkedButtonDropShadow, DS.Style.highlightedButtonDropShadow)
    property D.Palette innerShadowColor1: selectValue(DS.Style.buttonInnerShadow1, DS.Style.checkedButtonInnerShadow, DS.Style.highlightedButtonInnerShadow1)
    property D.Palette innerShadowColor2: selectValue(DS.Style.buttonInnerShadow2, null, DS.Style.highlightedButtonInnerShadow2)
    visible: !button.flat || button.down || button.checked || button.highlighted || button.visualFocus || button.hovered

    function selectValue(normal, checked, highlighted) {
        if (button.checked) {
            return checked
        } else if ((typeof button.highlighted == "boolean") && button.highlighted) {
            return highlighted
        }

        return normal
    }

    BoxShadow {
        anchors.fill: backgroundRect
        shadowBlur: selectValue(control.D.ColorSelector.controlState === D.DTK.PressedState ? 4 : 6, 6, 4)
        shadowOffsetY: selectValue(control.D.ColorSelector.controlState === D.DTK.PressedState ? 2 : 4, 4, 4)
        shadowColor: control.D.ColorSelector.dropShadowColor
        cornerRadius: backgroundRect.radius
        visible: control.D.ColorSelector.family === D.Palette.CommonColor
    }

    FocusBoxBorder {
        anchors.fill: parent
        radius: backgroundRect.radius
        color: button.palette.highlight
        visible: button.visualFocus
    }

    Rectangle {
        id: backgroundRect
        property alias color1: control.color1
        property alias color2: control.color2
        D.ColorSelector.hovered: false

        Gradient {
            id: backgroundGradient
            // Use the backgroundRect's colorselecor can filter the hovered state.
            GradientStop { position: 0.0; color: backgroundRect.D.ColorSelector.color1}
            GradientStop { position: 0.96; color: backgroundRect.D.ColorSelector.color2}
        }

        anchors.fill: parent
        radius: control.radius
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

        Component.onCompleted: {
            button.hoveredChanged.connect(function () {
                if (button.hovered) {
                    var pos = D.DTK.cursorPosition()
                    hoverAnimation.centerPoint = hoverAnimation.mapFromGlobal(pos.x, pos.y)
                    hoverAnimation.start()
                } else {
                    hoverAnimation.stop()
                }
            })
        }
    }

    BoxInsetShadow {
        anchors.fill: backgroundRect
        shadowBlur: 2
        shadowOffsetY: selectValue(control.D.ColorSelector.controlState === D.DTK.HoveredState ? -3 : -1, -1, -1)
        spread: 1
        shadowColor: control.D.ColorSelector.innerShadowColor1
        cornerRadius: backgroundRect.radius
        visible: innerShadowColor1 && shadowColor.a !== 0 && control.D.ColorSelector.family === D.Palette.CommonColor
    }

    BoxInsetShadow {
        id: innerShadow2
        anchors.fill: backgroundRect
        shadowBlur: 1
        shadowOffsetY: 1
        shadowColor: control.D.ColorSelector.innerShadowColor2
        cornerRadius: backgroundRect.radius
        visible: innerShadowColor2 && shadowColor.a !== 0 && control.D.ColorSelector.family === D.Palette.CommonColor
    }

    InsideBoxBorder {
        anchors.fill: backgroundRect
        radius: backgroundRect.radius
        visible: insideBorderColor
        color: control.D.ColorSelector.insideBorderColor
        borderWidth: DS.Style.control.borderWidth
    }

    OutsideBoxBorder {
        anchors.fill: backgroundRect
        radius: backgroundRect.radius
        visible: outsideBorderColor
        color: control.D.ColorSelector.outsideBorderColor
        borderWidth: DS.Style.control.borderWidth
    }

}
