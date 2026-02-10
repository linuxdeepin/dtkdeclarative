// SPDX-FileCopyrightText: 2021 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

import QtQuick
import org.deepin.dtk 1.0 as D
import org.deepin.dtk.style 1.0 as DS

// Use Item instead of ToolTip(Popup) so it stays in the visual tree,
// scrolls with content and gets clipped properly.
Item {
    id: control
    property Item target
    property string text
    property alias font: contentText.font
    property int timeout: 0

    x: 0
    y: target ? target.height + DS.Style.control.spacing : 0
    z: D.DTK.TopOrder

    readonly property real __naturalWidth: Math.max(DS.Style.alertToolTip.width,
                                                    contentText.implicitWidth + DS.Style.alertToolTip.horizontalPadding * 2)
    implicitWidth: target ? Math.min(__naturalWidth, target.width) : __naturalWidth
    implicitHeight: Math.max(DS.Style.alertToolTip.height,
                             contentText.implicitHeight + DS.Style.alertToolTip.verticalPadding * 2)
    width: implicitWidth
    height: implicitHeight

    Timer {
        interval: control.timeout
        running: control.timeout > 0 && control.visible
        onTriggered: control.visible = false
    }

    FloatingPanel {
        anchors.fill: parent
        radius: DS.Style.alertToolTip.radius
        implicitWidth: DS.Style.alertToolTip.width
        implicitHeight: DS.Style.alertToolTip.height
        backgroundColor: DS.Style.alertToolTip.background
        insideBorderColor: DS.Style.alertToolTip.insideBorder
        outsideBorderColor: DS.Style.alertToolTip.outsideBorder
    }

    Text {
        id: contentText
        property D.Palette textColor: DS.Style.alertToolTip.text
        anchors.fill: parent
        anchors.topMargin: DS.Style.alertToolTip.verticalPadding
        anchors.bottomMargin: DS.Style.alertToolTip.verticalPadding
        anchors.leftMargin: DS.Style.alertToolTip.horizontalPadding
        anchors.rightMargin: DS.Style.alertToolTip.horizontalPadding
        horizontalAlignment: Text.AlignLeft
        verticalAlignment: Text.AlignVCenter
        text: control.text
        color: D.ColorSelector.textColor
        wrapMode: Text.Wrap
    }

    BoxShadow {
        id: line
        property D.Palette dropShadowColor: DS.Style.alertToolTip.connecterdropShadow
        property D.Palette backgroundColor: DS.Style.alertToolTip.connecterBackground
        property D.Palette borderColor: DS.Style.control.border
        y: - height * (0.75)
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
