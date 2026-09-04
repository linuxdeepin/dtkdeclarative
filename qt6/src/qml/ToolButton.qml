// SPDX-FileCopyrightText: 2021 - 2026 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

import QtQuick
import QtQuick.Templates as T
import org.deepin.dtk 1.0 as D
import org.deepin.dtk.style 1.0 as DS
import org.deepin.dtk.private 1.0 as P

T.ToolButton {
    id: control
    // The checked chip is now a dedicated ToolButton style, so the checked
    // text palette stays consistent whether animations are enabled or not.
    property D.Palette textColor: checked ? DS.Style.toolButton.checkedText : (highlighted ? DS.Style.highlightedButton.text : DS.Style.button.text)

    property D.Palette checkedShadow: DS.Style.toolButton.checkedShadow

    implicitWidth: DS.Style.control.implicitWidth(control)
    implicitHeight: DS.Style.control.implicitHeight(control)
    topPadding: DS.Style.button.vPadding
    bottomPadding: DS.Style.button.vPadding
    leftPadding: DS.Style.button.hPadding
    rightPadding: DS.Style.button.hPadding
    spacing: DS.Style.control.spacing
    opacity: D.ColorSelector.controlState === D.DTK.DisabledState ? 0.4 : 1
    D.DciIcon.mode: D.ColorSelector.controlState
    D.DciIcon.theme: D.ColorSelector.controlTheme
    D.DciIcon.palette: D.DTK.makeIconPalette(palette)
    palette.windowText: D.ColorSelector.textColor
    D.ColorSelector.family: D.Palette.CrystalColor
    display: D.IconLabel.TextUnderIcon
    font: icon.name ? D.DTK.fontManager.t10: undefined
    flat: true

    icon {
        width: DS.Style.toolButton.iconSize
        height: DS.Style.toolButton.iconSize
        color: D.ColorSelector.textColor
    }

    contentItem: Item {
        implicitWidth: content.implicitWidth + (indicator ? indicator.width : 0)
        implicitHeight: content.implicitHeight
        D.IconLabel {
            id: content
            height: parent.height
            width: parent.width - (indicator ? indicator.width : 0)
            spacing: control.spacing
            mirrored: control.mirrored
            display: control.display
            alignment: indicator ? Qt.AlignLeft | Qt.AlignVCenter : Qt.AlignCenter
            text: control.text
            font: control.font
            color: control.D.ColorSelector.textColor
            icon: D.DTK.makeIcon(control.icon, control.D.DciIcon)
        }
        function updateIndicatorAnchors()
        {
            if (!indicator)
                return

            indicator.anchors.verticalCenter = control.verticalCenter
            indicator.anchors.right = control.right
            indicator.anchors.rightMargin = DS.Style.toolButton.indicatorRightMargin
        }
        Component.onCompleted: {
            updateIndicatorAnchors()
            control.indicatorChanged.connect(updateIndicatorAnchors)
        }
    }

    states: [
        State {
            name: "normal"
            when: !control.hovered && !control.checked && D.DTK.hasAnimation
            PropertyChanges {
                target: background
                scale : 0.9
            }

        },
        State {
            name: "hovered"
            when: control.hovered && !control.checked && D.DTK.hasAnimation
            PropertyChanges {
                target: contentItem
                scale : 1.1
            }
            PropertyChanges {
                target: background
                scale : 1.0
            }

        },
        State {
            name: "checked"
            when: control.checked && D.DTK.hasAnimation
            PropertyChanges {
                target: contentItem
                scale : 1.0
            }
        }
    ]

    transitions: Transition {
        NumberAnimation { properties: "scale"; easing.type: Easing.InOutQuad }
    }

    background: P.ButtonPanel {
        id: buttonPanel
        implicitWidth: DS.Style.toolButton.width
        implicitHeight: DS.Style.toolButton.height
        button: control
        outsideBorderColor: null
        insideBorderColor: null
        radius: DS.Style.toolButton.radius

        // Non-checked tool buttons: bind color1 to the background palette
        // which has transparent 'normal', tinted 'hovered' and 'pressed'
        // states. The ColorSelector picks the state automatically, and
        // ButtonPanel's default visibility shows the panel when hovered
        // or pressed (flat buttons are hidden in normal state).
        Binding on color1 {
            when: !control.checked
            value: DS.Style.toolButton.background
        }
        Binding on color2 {
            when: !control.checked
            value: buttonPanel.color1
        }

        // Dark-theme hover frosted-glass chip for non-checked tool buttons:
        // backdrop blur (radius 15, saturation 100%) plus a 1px white top
        // inset highlight and 1px black bottom inset shadow, over the
        // translucent dark tint from background (rgba(20,20,20,0.2)).
        // The blur is platform-gated; the inset bevel shows regardless.
        readonly property bool __darkHover: !control.checked
            && D.DTK.themeType === D.ApplicationHelper.DarkType
            && buttonPanel.D.ColorSelector.controlState === D.DTK.HoveredState

        D.InWindowBlur {
            id: hoverBlur
            anchors.fill: parent
            radius: 15
            saturation: 1.0
            offscreen: true
            visible: buttonPanel.__darkHover && hoverBlur.valid
            z: -1

            D.ItemViewport {
                anchors.fill: parent
                fixed: true
                sourceItem: hoverBlur.content
                radius: buttonPanel.radius
                hideSource: false
            }
        }

        // Inner shadow 1: 1px white top inset highlight.
        D.BoxInsetShadow {
            anchors.fill: parent
            visible: buttonPanel.__darkHover
            z: D.DTK.AboveOrder
            cornerRadius: buttonPanel.radius
            shadowColor: Qt.rgba(1, 1, 1, 0.1)
            shadowOffsetX: 0
            shadowOffsetY: 1
            shadowBlur: 1
        }

        // Inner shadow 2: 1px black bottom inset shadow.
        D.BoxInsetShadow {
            anchors.fill: parent
            visible: buttonPanel.__darkHover
            z: D.DTK.AboveOrder
            cornerRadius: buttonPanel.radius
            shadowColor: Qt.rgba(0, 0, 0, 0.5)
            shadowOffsetX: 0
            shadowOffsetY: -1
            shadowBlur: 1
        }

        // Checked tool buttons use a subtle overlay chip with an accent icon
        // instead of the shared accent-fill checked button style.
        Binding on color1 {
            when: control.checked
            value: DS.Style.toolButton.checkedBackground
        }
        Binding on color2 {
            when: control.checked
            value: DS.Style.toolButton.checkedBackground
        }
        // Inset bottom shadow for the checked chip.
        D.BoxInsetShadow {
            anchors.fill: parent
            visible: control.checked
            z: D.DTK.AboveOrder
            cornerRadius: buttonPanel.radius
            shadowColor: control.D.ColorSelector.checkedShadow
            shadowOffsetX: 0
            shadowOffsetY: -1
            shadowBlur: 1
        }
    }
}
