// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

import QtQuick 2.11
import org.deepin.dtk.impl 1.0 as D
import org.deepin.dtk.style 1.0 as DS

Control {
    id: control

    padding: DS.Style.floatingMessage.panel.radius / 2

    property D.Palette backgroundColor: DS.Style.floatingMessage.panel.background
    property D.Palette dropShadowColor: DS.Style.floatingMessage.panel.dropShadow
    property D.Palette outsideBorderColor: DS.Style.floatingMessage.panel.outsideBorder
    property D.Palette insideBorderColor: DS.Style.floatingMessage.panel.insideBorder
    // corner radius
    property int radius: DS.Style.floatingMessage.panel.radius
    // blur radius
    property int blurRadius: DS.Style.floatingMessage.panel.radius

    background: D.InWindowBlur {
        implicitWidth: DS.Style.floatingMessage.panel.width
        implicitHeight: DS.Style.floatingMessage.panel.height
        radius: blurRadius
        offscreen: true

        D.ItemViewport {
            anchors.fill: parent
            fixed: true
            sourceItem: parent
            radius: control.radius
            hideSource: false
        }

        BoxShadow {
            anchors.fill: backgroundRect
            shadowOffsetX: 0
            shadowOffsetY: 6
            shadowColor: control.D.ColorSelector.dropShadowColor
            shadowBlur: 20
            cornerRadius: backgroundRect.radius
            spread: 0
            hollow: true
        }

        Rectangle {
            id: backgroundRect
            anchors.fill: parent
            radius: control.radius
            color: control.D.ColorSelector.backgroundColor
        }

        Loader {
            anchors.fill: backgroundRect
            active: control.D.ColorSelector.controlTheme === D.ApplicationHelper.DarkType
            sourceComponent: InsideBoxBorder {
                radius: backgroundRect.radius
                color: control.D.ColorSelector.insideBorderColor
                borderWidth: DS.Style.control.borderWidth
            }
        }

        OutsideBoxBorder {
            anchors.fill: backgroundRect
            radius: backgroundRect.radius
            color: control.D.ColorSelector.outsideBorderColor
            borderWidth: DS.Style.control.borderWidth
        }
    }
}
