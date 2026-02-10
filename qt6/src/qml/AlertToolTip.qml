// SPDX-FileCopyrightText: 2021 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

import QtQuick
import org.deepin.dtk 1.0 as D
import org.deepin.dtk.style 1.0 as DS

// AlertToolTip is implemented with Control (not Popup) so it stays in the visual
// tree, scrolls with content and gets clipped properly. Enter/exit animation
// is driven by the visible state.
Control {
    id: control
    property Item target
    property string text
    property alias font: contentText.font
    property int timeout: 0

    x: 0
    y: _displayY
    z: D.DTK.TopOrder

    topPadding: DS.Style.alertToolTip.verticalPadding
    bottomPadding: DS.Style.alertToolTip.verticalPadding
    leftPadding: DS.Style.alertToolTip.horizontalPadding
    rightPadding: DS.Style.alertToolTip.horizontalPadding

    readonly property real __naturalWidth: Math.max(DS.Style.alertToolTip.width,
                                                    contentText.implicitWidth + DS.Style.alertToolTip.horizontalPadding * 2)
    implicitWidth: target ? Math.min(__naturalWidth, target.width) : __naturalWidth
    implicitHeight: Math.max(DS.Style.alertToolTip.height,
                             contentText.implicitHeight + DS.Style.alertToolTip.verticalPadding * 2)
    width: implicitWidth
    height: implicitHeight

    // Animated y so tip stays attached to target; enter = from below target to final, exit = back.
    property real _displayY: target ? target.height : 0
    function _updateDisplayY() {
        if (target) {
            if (visible)
                _displayY = target.height + DS.Style.control.spacing
            else
                _displayY = target.height
        }
    }
    onVisibleChanged: _updateDisplayY()
    onTargetChanged: _updateDisplayY()
    Connections {
        target: control.target
        function onHeightChanged() { control._updateDisplayY() }
    }
    Component.onCompleted: _updateDisplayY()
    Behavior on _displayY {
        NumberAnimation { duration: 200 }
    }

    Timer {
        interval: control.timeout
        running: control.timeout > 0 && control.visible
        onTriggered: control.visible = false
    }

    background: Item {
        implicitWidth: DS.Style.alertToolTip.width
        implicitHeight: DS.Style.alertToolTip.height

        FloatingPanel {
            anchors.fill: parent
            radius: DS.Style.alertToolTip.radius
            implicitWidth: DS.Style.alertToolTip.width
            implicitHeight: DS.Style.alertToolTip.height
            backgroundColor: DS.Style.alertToolTip.background
            insideBorderColor: DS.Style.alertToolTip.insideBorder
            outsideBorderColor: DS.Style.alertToolTip.outsideBorder
        }

        BoxShadow {
            id: connector
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
                color: connector.D.ColorSelector.backgroundColor
                border.color: connector.D.ColorSelector.borderColor
                border.width: 1
            }
        }
    }

    contentItem: Text {
        id: contentText
        property D.Palette textColor: DS.Style.alertToolTip.text
        horizontalAlignment: Text.AlignLeft
        verticalAlignment: Text.AlignVCenter
        text: control.text
        font: control.font
        color: D.ColorSelector.textColor
        wrapMode: Text.Wrap
    }
}
