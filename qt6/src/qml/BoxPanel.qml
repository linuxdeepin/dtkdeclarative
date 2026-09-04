// SPDX-FileCopyrightText: 2022-2026 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

import QtQuick
import org.deepin.dtk 1.0 as D
import org.deepin.dtk.style 1.0 as DS

Item {
    id: control

    property int radius: DS.Style.control.radius
    property D.Palette color1: DS.Style.button.background1
    property D.Palette color2: DS.Style.button.background2
    property D.Palette insideBorderColor: DS.Style.button.insideBorder
    property D.Palette outsideBorderColor: DS.Style.button.outsideBorder
    property D.Palette dropShadowColor: DS.Style.button.dropShadow
    property D.Palette dropShadowColor2: null
    property D.Palette innerShadowColor1: DS.Style.button.innerShadow1
    property D.Palette innerShadowColor2: DS.Style.button.innerShadow2
    property int boxShadowBlur: 6
    property int boxShadowOffsetY: 4
    property int boxShadowOffsetY2: 0
    property int innerShadowOffsetY1: -1
    property int innerShadowOffsetY2: 1
    // Background color changes with hover state if `backgroundFlowingHovered` is `true`.
    property bool backgroundFlowsHovered: true
    property bool enableBoxShadow: control.D.ColorSelector.family === D.Palette.CommonColor
    // Shadow and gradient rendering is opt-in. Commit 52633cb temporarily
    // dropped BoxPanel's drop shadow, inner shadow and gradient for every
    // consumer, so they stay off by default here. Only the normal text
    // Button (see Button.qml) turns them on, which keeps the other button
    // styles and panels looking as they did before.
    property bool enableDropShadow: false
    property bool enableInnerShadow: false
    property bool enableGradient: false

    // Hard drop shadows (blur == 0): two rounded Rectangles matching the
    // border-box, placed below the background and borders in z-order so the
    // button's own paint naturally covers the overlapping part. Only the
    // strip extending below the button is visible, with full rounded corners.
    Rectangle {
        id: hardShadow
        visible: control.enableBoxShadow && control.enableDropShadow
                && control.boxShadowBlur === 0
                && dropShadowColor && control.D.ColorSelector.dropShadowColor.a > 0
        y: control.boxShadowOffsetY
        width: backgroundRect.width
        height: backgroundRect.height
        radius: backgroundRect.radius
        color: control.D.ColorSelector.dropShadowColor
        antialiasing: false
    }

    Rectangle {
        id: hardShadow2
        visible: control.enableBoxShadow && control.enableDropShadow
                && control.boxShadowBlur === 0
                && dropShadowColor2 && control.D.ColorSelector.dropShadowColor2.a > 0
                && control.boxShadowOffsetY2 > 0
        y: control.boxShadowOffsetY2
        width: backgroundRect.width
        height: backgroundRect.height
        radius: backgroundRect.radius
        color: control.D.ColorSelector.dropShadowColor2
        antialiasing: false
    }

    // Soft drop shadow (blur > 0): uses BoxShadow for blurred rendering.
    // Active only when boxShadowBlur is non-zero (e.g. pressed state).
    Loader {
        active: control.enableBoxShadow && control.enableDropShadow
                && control.boxShadowBlur > 0
                && dropShadowColor && control.D.ColorSelector.dropShadowColor.a > 0
        anchors.fill: parent

        sourceComponent: BoxShadow {
            cornerRadius: backgroundRect.radius
            shadowBlur: control.boxShadowBlur
            shadowOffsetY: control.boxShadowOffsetY
            shadowColor: control.D.ColorSelector.dropShadowColor
        }
    }

    Rectangle {
        id: backgroundRect
        property alias color1: control.color1
        property alias color2: control.color2
        D.ColorSelector.hovered: backgroundFlowsHovered ? undefined : false
        objectName: "background"

        Gradient {
            id: backgroundGradient
            // Use the backgroundRect's colorselecor can filter the hovered state.
            GradientStop { position: 0.0; color: backgroundRect.D.ColorSelector.color1}
            GradientStop { position: 1.0; color: backgroundRect.D.ColorSelector.color2}
        }

        anchors.fill: parent
        radius: control.radius
        gradient: control.enableGradient
                  && D.ColorSelector.color1 !== D.ColorSelector.color2 ? backgroundGradient : null
        color: D.ColorSelector.color1
    }

    Loader {
        // Extend 1px below the panel so the bottom inner shadow lands on the
        // button edge.  The panel is inset 1px (see Button.qml insets), so
        // without this the shadow sits on the panel bottom, leaving a visible
        // gap and rendering sub-pixel thin at certain DPR values.
        anchors.fill: backgroundRect
        anchors.bottomMargin: -1
        readonly property color innerShadowColor: control.D.ColorSelector.innerShadowColor1
        active: control.enableBoxShadow && control.enableInnerShadow
                && innerShadowColor1 && innerShadowColor.a !== 0
        z: D.DTK.AboveOrder

        sourceComponent: BoxInsetShadow {
            shadowBlur: 1
            shadowOffsetY: control.innerShadowOffsetY1
            spread: 0
            shadowColor: innerShadowColor
            cornerRadius: backgroundRect.radius
        }
    }

    Loader {
        anchors.fill: backgroundRect
        readonly property color innerShadowColor: control.D.ColorSelector.innerShadowColor2
        active: control.enableBoxShadow && control.enableInnerShadow
                && innerShadowColor2 && innerShadowColor.a !== 0
        z: D.DTK.AboveOrder

        sourceComponent: BoxInsetShadow {
            shadowBlur: 1
            shadowOffsetY: control.innerShadowOffsetY2
            shadowColor: innerShadowColor
            cornerRadius: backgroundRect.radius
        }
    }

    Loader {
        active: insideBorderColor
        anchors.fill: backgroundRect
        z: D.DTK.AboveOrder

        sourceComponent: InsideBoxBorder {
            radius: backgroundRect.radius
            color: control.D.ColorSelector.insideBorderColor
            borderWidth: DS.Style.control.borderWidth
        }
    }

    Loader {
        active: outsideBorderColor
        anchors.fill: backgroundRect
        z: D.DTK.AboveOrder

        sourceComponent: OutsideBoxBorder {
            radius: backgroundRect.radius
            color: control.D.ColorSelector.outsideBorderColor
            borderWidth: DS.Style.control.borderWidth
        }
    }
}
