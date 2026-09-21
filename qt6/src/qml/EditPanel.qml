// SPDX-FileCopyrightText: 2021 - 2026 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

import QtQuick
import org.deepin.dtk 1.0 as D
import org.deepin.dtk.style 1.0 as DS


Item {
    id: panel
    property Item control
    property D.Palette backgroundColor: DS.Style.edit.background
    property D.Palette alertBackgroundColor: DS.Style.edit.alertBackground
    property color color: panel.showAlert ? panel.D.ColorSelector.alertBackgroundColor
                                            : panel.D.ColorSelector.backgroundColor
    property D.Palette outsideBorderColor: DS.Style.edit.outsideBorder
    property D.Palette insideBorderColor: DS.Style.edit.insideBorder
    property D.Palette innerShadowColor: DS.Style.edit.innerShadow
    property D.Palette dropShadowColor: DS.Style.edit.dropShadow
    property alias showBorder: _border.active
    property bool showAlert: false
    property string alertText: ""
    property int alertDuration: 0

    readonly property real radius: DS.Style.button.radius

    // Outer drop shadow: rendered below the background so only the
    // 1px strip extending past the bottom edge is visible.
    Loader {
        active: dropShadowColor
        anchors.fill: parent

        sourceComponent: BoxShadow {
            cornerRadius: panel.radius
            shadowBlur: 1
            shadowOffsetY: 1
            hollow: true
            shadowColor: panel.D.ColorSelector.dropShadowColor
        }
    }

    // Background fill: rendered on top of the drop shadow, covering
    // the overlapping area.
    Rectangle {
        id: background
        anchors.fill: parent
        radius: panel.radius
        color: panel.color
    }

    Loader {
        active: outsideBorderColor
        anchors.fill: parent

        sourceComponent: OutsideBoxBorder {
            radius: panel.radius
            color: panel.D.ColorSelector.outsideBorderColor
            borderWidth: DS.Style.control.borderWidth
        }
    }

    Loader {
        active: insideBorderColor
        anchors.fill: parent

        sourceComponent: InsideBoxBorder {
            radius: panel.radius
            color: panel.D.ColorSelector.insideBorderColor
        }
    }

    // Inner shadow: 1px inset at the top edge.
    Loader {
        active: innerShadowColor
        anchors.fill: parent

        sourceComponent: BoxInsetShadow {
            shadowBlur: 1
            shadowOffsetY: -1
            spread: 0
            shadowColor: panel.D.ColorSelector.innerShadowColor
            cornerRadius: panel.radius
        }
    }

    Loader {
        id: _border
        anchors.fill: parent
        active: control.activeFocus
        sourceComponent: FocusBoxBorder {
            color: control.palette.highlight
            radius: panel.radius
        }
    }

    // Keep Loader active while there is alert text so we don't destroy/recreate when
    // caller toggles showAlert to refresh the message; avoids wrong text (e.g. "systemd journal")
    // from binding context during recreation.
    Loader {
        active: alertText.length !== 0
        sourceComponent: AlertToolTip {
            target: control
            timeout: alertDuration
            visible: showAlert
            text: alertText
        }
    }
}
