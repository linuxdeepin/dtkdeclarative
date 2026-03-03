// SPDX-FileCopyrightText: 2021 - 2026 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

import QtQuick
import org.deepin.dtk 1.0 as D
import org.deepin.dtk.style 1.0 as DS

Control {
    id: control
    property Item target
    property string text
    property int timeout: 0
    property bool _expired: false
    readonly property bool _shown: control.visible && !_expired

    x: 0
    y: (target ? target.height : 0) + (_shown ? DS.Style.control.spacing : 0)
    Behavior on y {
        NumberAnimation { duration: 200 }
    }
    opacity: _shown ? 1 : 0
    enabled: _shown
    topPadding: DS.Style.alertToolTip.verticalPadding
    bottomPadding: DS.Style.alertToolTip.verticalPadding
    leftPadding: DS.Style.alertToolTip.horizontalPadding
    rightPadding: DS.Style.alertToolTip.horizontalPadding
    implicitWidth: target ? Math.min(DS.Style.control.implicitWidth(control), target.width) : DS.Style.control.implicitWidth(control)
    implicitHeight: DS.Style.control.implicitHeight(control)
    z: D.DTK.TopOrder

    Timer {
        id: autoHideTimer
        interval: control.timeout
        running: control.timeout > 0 && control.visible && !control._expired
        onTriggered: control._expired = true
    }

    onVisibleChanged: {
        _expired = false
        if (visible && timeout > 0)
            autoHideTimer.restart()
    }

    onTextChanged: {
        _expired = false
        if (visible && timeout > 0)
            autoHideTimer.restart()
    }

    background: FloatingPanel {
        radius: DS.Style.alertToolTip.radius
        implicitWidth: DS.Style.alertToolTip.width
        implicitHeight: DS.Style.alertToolTip.height
        backgroundColor: DS.Style.alertToolTip.background
        insideBorderColor: DS.Style.alertToolTip.insideBorder
        outsideBorderColor: DS.Style.alertToolTip.outsideBorder
    }

    contentItem: Text {
        property D.Palette textColor: DS.Style.alertToolTip.text
        horizontalAlignment: Text.AlignLeft
        verticalAlignment: Text.AlignVCenter
        text: control.text
        font: control.font
        color: D.ColorSelector.textColor
        wrapMode: Text.Wrap
    }

    BoxShadow {
        id: line
        property D.Palette dropShadowColor: DS.Style.alertToolTip.connecterdropShadow
        property D.Palette backgroundColor: DS.Style.alertToolTip.connecterBackground
        property D.Palette borderColor: DS.Style.control.border
        y: -height * 0.75
        width: DS.Style.alertToolTip.connectorWidth
        height: DS.Style.alertToolTip.connectorHeight
        shadowBlur: 4
        shadowOffsetY: 2
        shadowColor: D.ColorSelector.dropShadowColor
        cornerRadius: DS.Style.control.radius

        Rectangle {
            anchors.fill: parent
            color: line.D.ColorSelector.backgroundColor
            border.color: line.D.ColorSelector.borderColor
            border.width: 1
        }
    }
}
