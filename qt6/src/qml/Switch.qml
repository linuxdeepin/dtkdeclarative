// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

import QtQuick 2.11
import QtQuick.Templates as T
import org.deepin.dtk 1.0 as D
import org.deepin.dtk.style 1.0 as DS

T.Switch {
    id: control

    property D.Palette backgroundColor: DS.Style.switchButton.background
    property D.Palette handleColor: DS.Style.switchButton.handle

    implicitWidth: DS.Style.control.implicitWidth(control)
    implicitHeight: DS.Style.control.implicitHeight(control)

    baselineOffset: contentItem.y + contentItem.baselineOffset
    topPadding: DS.Style.control.vPadding
    bottomPadding: DS.Style.control.vPadding
    spacing: DS.Style.control.spacing
    D.ColorSelector.hovered: false // disable hover ==> normal animation

    indicator: D.DciIcon {
        id: handle
        implicitWidth: DS.Style.switchButton.indicatorWidth
        implicitHeight: DS.Style.switchButton.indicatorHeight

        x: text ? (control.mirrored ? control.width - width - control.rightPadding : control.leftPadding) : control.leftPadding + (control.availableWidth - width) / 2
        y: control.topPadding + (control.availableHeight - height) / 2

        width: DS.Style.switchButton.handleWidth
        height: DS.Style.switchButton.handleHeight
        sourceSize: Qt.size(DS.Style.switchButton.indicatorWidth, DS.Style.switchButton.indicatorWidth)
        name: !control.checked ? "switch_on" : "switch_off"
        opacity: control.D.ColorSelector.controlState === D.DTK.DisabledState && control.checked ? 0.4 : 1
        palette: DTK.makeIconPalette(control.palette)
        mode: control.D.ColorSelector.controlState
        theme: control.D.ColorSelector.controlTheme
        fallbackToQIcon: false
    }

    Timer {
        id: toggletimer
        interval: 300
        onTriggered: {
            control.toggle()
        }
    }

    function palyAndSetImage() {
        handle.play(D.DTK.NormalState)
        toggletimer.start();
    }

    Keys.onSpacePressed: palyAndSetImage()
    Keys.onEnterPressed: palyAndSetImage()
    Keys.onReturnPressed: palyAndSetImage()

    MouseArea {
        anchors.fill: parent
        acceptedButtons: Qt.LeftButton
        onClicked: palyAndSetImage()
    }

    contentItem: Label {
        leftPadding: control.indicator && !control.mirrored ? control.indicator.width + control.spacing : 0
        rightPadding: control.indicator && control.mirrored ? control.indicator.width + control.spacing : 0

        text: control.text
        verticalAlignment: Text.AlignVCenter
    }
}
