// SPDX-FileCopyrightText: 2021-2026 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

import QtQuick
import QtQuick.Templates as T
import org.deepin.dtk 1.0 as D
import org.deepin.dtk.style 1.0 as DS
import org.deepin.dtk.private 1.0 as P

T.Button {
    id: control

    property D.Palette textColor: checked ? DS.Style.checkedButton.text : (highlighted ? DS.Style.highlightedButton.text : DS.Style.button.text)
    implicitWidth: DS.Style.control.implicitWidth(control)
    implicitHeight: DS.Style.control.implicitHeight(control)

    topPadding: DS.Style.button.vPadding
    bottomPadding: DS.Style.button.vPadding
    leftPadding: DS.Style.button.hPadding
    rightPadding: DS.Style.button.hPadding
    spacing: DS.Style.control.spacing
    // Reserve 1px on each side for the outside border so it is never clipped
    // by a parent with clip:true (e.g. ListView). Checked/highlighted buttons
    // have no outside border, so they keep 0 insets.
    leftInset: (checked || highlighted) ? 0 : 1
    rightInset: (checked || highlighted) ? 0 : 1
    topInset: (checked || highlighted) ? 0 : 1
    bottomInset: (checked || highlighted) ? 0 : 1
    opacity: D.ColorSelector.controlState === D.DTK.DisabledState ? 0.4 : 1
    D.DciIcon.mode: D.ColorSelector.controlState
    D.DciIcon.theme: D.ColorSelector.controlTheme
    D.DciIcon.palette: D.DTK.makeIconPalette(palette)
    palette.windowText: D.ColorSelector.textColor
    icon {
        width: DS.Style.button.iconSize
        height: DS.Style.button.iconSize
        color: D.ColorSelector.textColor
    }

    background: P.ButtonPanel {
        id: buttonPanel
        implicitWidth: DS.Style.button.width
        implicitHeight: DS.Style.button.height
        button: control
        // The normal (non-checked, non-highlighted) text button opts into the
        // BoxPanel drop shadow, inner shadow and gradient that were dropped
        // for every consumer in commit 52633cb. Checked/highlighted buttons
        // and all other ButtonPanel users keep their existing flat look.
        radius: control.checked || control.highlighted ? DS.Style.control.radius : DS.Style.button.radius
        enableDropShadow: !(control.checked || control.highlighted)
        enableInnerShadow: !(control.checked || control.highlighted)
        enableGradient: !(control.checked || control.highlighted)
        // Crystal dark-mode needs box shadow enabled so the inner bevel
        // (direct BoxInsetShadow in BoxPanel) renders. State-level gating
        // (pressed hides the bevel) is handled in BoxPanel, not here.
        enableBoxShadow: !(control.checked || control.highlighted)
            && (buttonPanel.D.ColorSelector.family === D.Palette.CommonColor
                || buttonPanel.__crystalDark)

        // Crystal backdrop blur behind the translucent tint produces a
        // frosted-glass chip. Active in all non-disabled crystal states;
        // light theme additionally excludes the inactive state.
        // Mirrors the ToolButton hover blur (radius 15, saturation 1.0,
        // offscreen itemViewport).
        readonly property bool __crystalBlur:
            !control.checked && !control.highlighted
            && buttonPanel.D.ColorSelector.family === D.Palette.CrystalColor
            && buttonPanel.D.ColorSelector.controlState !== D.DTK.DisabledState
            && (D.DTK.themeType === D.ApplicationHelper.DarkType
                || buttonPanel.D.ColorSelector.controlState !== D.DTK.InactiveState)

        D.InWindowBlur {
            id: crystalBlur
            anchors.fill: parent
            radius: 15
            saturation: 1.0
            offscreen: true
            visible: buttonPanel.__crystalBlur && crystalBlur.valid
            z: -1

            D.ItemViewport {
                anchors.fill: parent
                fixed: true
                sourceItem: crystalBlur.content
                radius: buttonPanel.radius
                hideSource: false
            }
        }
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
        }
        Component.onCompleted: {
            updateIndicatorAnchors()
            control.indicatorChanged.connect(updateIndicatorAnchors)
        }
    }
}
