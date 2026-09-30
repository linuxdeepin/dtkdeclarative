// SPDX-FileCopyrightText: 2022 - 2026 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

import QtQuick
import org.deepin.dtk 1.0 as D
import org.deepin.dtk.style 1.0 as DS

Item {
    id: panel

    property D.Palette backgroundColor: DS.Style.highlightPanel.background
    property D.Palette outerShadowColor: DS.Style.highlightPanel.dropShadow
    property D.Palette innerShadowColor: DS.Style.highlightPanel.innerShadow
    // Dual-direction inner shadow palettes for consumers that need a
    // top + bottom bevel (e.g. MenuItem hover).  Default to null so the
    // bevel Loaders stay inactive and existing callers are unaffected.
    property D.Palette bevelShadowColor1: null
    property D.Palette bevelShadowColor2: null
    property int radius: DS.Style.highlightPanel.radius

    implicitWidth: DS.Style.highlightPanel.width
    implicitHeight: DS.Style.highlightPanel.height
    // TODO drop shadow temporarily.
    // BoxShadow {
    //     anchors.fill: backgroundRect
    //     visible: panel.outerShadowColor
    //     shadowColor: panel.D.ColorSelector.outerShadowColor
    //     shadowOffsetY: 4
    //     shadowBlur: 6
    //     cornerRadius: backgroundRect.radius
    // }

    Rectangle {
        id: backgroundRect
        anchors.fill: parent
        color: panel.D.ColorSelector.backgroundColor
        radius: panel.radius
    }

    // Bottom inner shadow (legacy single-direction API).
    Loader {
        anchors.fill: backgroundRect
        active: panel.innerShadowColor

        sourceComponent: BoxInsetShadow {
            shadowColor: panel.D.ColorSelector.innerShadowColor
            shadowOffsetY: -1
            shadowBlur: 2
            spread: 1
            cornerRadius: backgroundRect.radius
        }
    }

    // Top inner shadow (bevel highlight).
    Loader {
        anchors.fill: backgroundRect
        active: panel.bevelShadowColor1

        sourceComponent: BoxInsetShadow {
            shadowColor: panel.D.ColorSelector.bevelShadowColor1
            shadowOffsetY: 1
            shadowBlur: 1
            cornerRadius: backgroundRect.radius
        }
    }

    // Bottom inner shadow (bevel shadow).
    Loader {
        anchors.fill: backgroundRect
        active: panel.bevelShadowColor2

        sourceComponent: BoxInsetShadow {
            shadowColor: panel.D.ColorSelector.bevelShadowColor2
            shadowOffsetY: -1
            shadowBlur: 1
            cornerRadius: backgroundRect.radius
        }
    }
}
