// SPDX-FileCopyrightText: 2022 - 2026 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

import QtQuick
import org.deepin.dtk 1.0 as D
import org.deepin.dtk.style 1.0 as DS
import org.deepin.dtk 1.0

BoxPanel {
    id: control
    property Item button

    property bool enableAnimation: control.D.ColorSelector.controlState === D.DTK.HoveredState && control.D.ColorSelector.family === D.Palette.CommonColor
    color1: selectValue(DS.Style.button.background1, DS.Style.checkedButton.background, DS.Style.highlightedButton.background1)
    color2: selectValue(DS.Style.button.background2, DS.Style.checkedButton.background, DS.Style.highlightedButton.background2)
    insideBorderColor: selectValue(DS.Style.button.insideBorder, null, DS.Style.highlightedButton.border)
    outsideBorderColor: selectValue(DS.Style.button.outsideBorder, null, null)
    dropShadowColor: selectValue(DS.Style.button.dropShadow, DS.Style.checkedButton.dropShadow, DS.Style.highlightedButton.dropShadow)
    dropShadowColor2: selectValue(DS.Style.button.dropShadow2, null, null)
    innerShadowColor1: selectValue(DS.Style.button.innerShadow1, DS.Style.checkedButton.innerShadow, DS.Style.highlightedButton.innerShadow1)
    innerShadowColor2: selectValue(DS.Style.button.innerShadow2, null, DS.Style.highlightedButton.innerShadow2)
    // All states use hard shadows (blur=0). Normal/hover: 2px far shadow +
    // 1px near shadow layered on top. Pressed: 1px only (CSS `0 1px 0 0`).
    boxShadowBlur: selectValue(0, 6, 4)
    boxShadowOffsetY: selectValue(control.D.ColorSelector.controlState === D.DTK.PressedState ? 1 : 2, 4, 4)
    boxShadowOffsetY2: selectValue(control.D.ColorSelector.controlState === D.DTK.PressedState ? 0 : 1, 0, 0)
    innerShadowOffsetY1: -1
    visible: !button.flat || button.checked || button.highlighted || button.visualFocus || control.D.ColorSelector.controlState === D.DTK.PressedState || control.D.ColorSelector.controlState === D.DTK.HoveredState

    function selectValue(normal, checked, highlighted) {
        if (button.checked) {
            return checked
        } else if ((typeof button.highlighted == "boolean") && button.highlighted) {
            return highlighted
        }

        return normal
    }

    Loader {
        anchors.fill: parent
        active: button.visualFocus

        sourceComponent: FocusBoxBorder {
            radius: control.radius
            color: button.palette.highlight
        }
    }

    Gradient {
        id: hoverBackgroundGradient
        GradientStop { position: 0.0; color: control.D.ColorSelector.color1 }
        GradientStop { position: 0.96; color: control.D.ColorSelector.color2 }
    }
}
