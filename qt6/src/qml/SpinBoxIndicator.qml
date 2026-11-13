// SPDX-FileCopyrightText: 2021 - 2026 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

import QtQuick
import org.deepin.dtk 1.0 as D
import org.deepin.dtk.style 1.0 as DS

Control {
    id: control
    enum IndicatorDirection {
        UpIndicator = 0,
        DownIndicator = 1
    }

    property Item spinBox
    property bool pressed
    property bool singleIndicator: false
    property int direction
    property D.Palette inactiveBackgroundColor: DS.Style.spinBox.indicator.background

    palette.windowText: control.D.ColorSelector.inactiveBackgroundColor
    opacity: D.ColorSelector.controlState === D.DTK.DisabledState ? 0.4 : 1
    hoverEnabled: true
    implicitWidth: DS.Style.spinBox.indicator.width
    implicitHeight: (spinBox.implicitHeight - 2 * DS.Style.spinBox.indicatorContainerVMargin) / 2

    ToolButton {
        anchors.fill: parent
        topPadding: 0
        bottomPadding: 0
        leftPadding: 0
        rightPadding: 0
        focusPolicy: Qt.NoFocus
        background: null
        icon {
            name: direction === SpinBoxIndicator.IndicatorDirection.UpIndicator ? "arrow_ordinary_up" : "arrow_ordinary_down"
            width: DS.Style.spinBox.indicator.focusIconSize
            height: DS.Style.spinBox.indicator.focusIconSize
        }
        activeFocusOnTab: false
        onClicked: {
            if (direction === SpinBoxIndicator.IndicatorDirection.UpIndicator) {
                spinBox.increase()
            } else {
                spinBox.decrease()
            }
        }
    }
}
