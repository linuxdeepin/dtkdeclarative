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
    property D.Palette indicatorBackground: DS.Style.spinBox.indicatorBackground
    property D.Palette indicatorInnerShadow: DS.Style.spinBox.indicatorInnerShadow

    palette.windowText: control.D.ColorSelector.inactiveBackgroundColor
    // Background stays full opacity; only the icon dims when disabled.
    hoverEnabled: true
    implicitWidth: DS.Style.spinBox.indicator.width
    implicitHeight: (spinBox.implicitHeight - 2 * DS.Style.spinBox.indicatorContainerVMargin) / 2

    // Each indicator draws its own half of the combined background.
    // The up indicator has rounded top corners; the down indicator has
    // rounded bottom corners. Together they form a seamless whole with
    // no overlap and no gap.
    background: Rectangle {
        radius: DS.Style.spinBox.indicatorContainerRadius
        color: control.D.ColorSelector.indicatorBackground
        topLeftRadius: direction === SpinBoxIndicator.IndicatorDirection.UpIndicator ? DS.Style.spinBox.indicatorContainerRadius : 0
        topRightRadius: direction === SpinBoxIndicator.IndicatorDirection.UpIndicator ? DS.Style.spinBox.indicatorContainerRadius : 0
        bottomLeftRadius: direction === SpinBoxIndicator.IndicatorDirection.DownIndicator ? DS.Style.spinBox.indicatorContainerRadius : 0
        bottomRightRadius: direction === SpinBoxIndicator.IndicatorDirection.DownIndicator ? DS.Style.spinBox.indicatorContainerRadius : 0

        Loader {
            active: D.DTK.themeType === D.ApplicationHelper.DarkType && direction === SpinBoxIndicator.IndicatorDirection.DownIndicator
            anchors.fill: parent
            sourceComponent: BoxInsetShadow {
                shadowBlur: 1
                shadowOffsetY: -1
                spread: 0
                shadowColor: control.D.ColorSelector.indicatorInnerShadow
                cornerRadius: DS.Style.spinBox.indicatorContainerRadius
            }
        }
    }

    ToolButton {
        opacity: D.ColorSelector.controlState === D.DTK.DisabledState ? 0.4 : 1
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
