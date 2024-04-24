// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

import QtQuick 2.11
import org.deepin.dtk 1.0 as D
import org.deepin.dtk.style 1.0 as DS

Control {
    id: control

    padding: DS.Style.floatingPanel.radius / 2

    property D.Palette backgroundColor: DS.Style.floatingPanel.background
    property D.Palette backgroundNoBlurColor: DS.Style.floatingPanel.backgroundNoBlur
    property D.Palette dropShadowColor: DS.Style.floatingPanel.dropShadow
    property D.Palette outsideBorderColor: DS.Style.floatingPanel.outsideBorder
    property D.Palette insideBorderColor: DS.Style.floatingPanel.insideBorder
    property D.Palette overlayColor: DS.Style.inWindowBlur.overlay
    // corner radius
    property int radius: DS.Style.floatingPanel.radius
    // blur radius
    property int blurRadius: 64

    background: D.InWindowBlur {
        id: blur
        implicitWidth: DS.Style.floatingPanel.width
        implicitHeight: DS.Style.floatingPanel.height
        radius: blurRadius
        offscreen: true
        overlay: Rectangle {
            radius: control.radius
            color: control.D.ColorSelector.overlayColor
        }

        D.ItemViewport {
            anchors.fill: parent
            fixed: true
            sourceItem: blur.content
            radius: control.radius
            hideSource: false
        }

        Loader {
            anchors.fill: backgroundRect
            active: control.dropShadowColor
            BoxShadow {
                shadowOffsetX: 0
                shadowOffsetY: 6
                shadowColor: control.D.ColorSelector.dropShadowColor
                shadowBlur: 20
                cornerRadius: backgroundRect.radius
                spread: 0
                hollow: true
            }
        }

        Rectangle {
            id: backgroundRect
            anchors.fill: parent
            radius: control.radius
            color: blur.valid ? control.D.ColorSelector.backgroundColor
                              : control.D.ColorSelector.backgroundNoBlurColor
        }

        Loader {
            anchors.fill: backgroundRect
            active: control.insideBorderColor && control.D.ColorSelector.controlTheme === D.ApplicationHelper.DarkType
            sourceComponent: InsideBoxBorder {
                radius: backgroundRect.radius
                color: control.D.ColorSelector.insideBorderColor
                borderWidth: DS.Style.control.borderWidth
            }
        }

        Loader {
            anchors.fill: backgroundRect
            active: control.outsideBorderColor
            OutsideBoxBorder {
                radius: backgroundRect.radius
                color: control.D.ColorSelector.outsideBorderColor
                borderWidth: DS.Style.control.borderWidth
            }
        }
    }
}
