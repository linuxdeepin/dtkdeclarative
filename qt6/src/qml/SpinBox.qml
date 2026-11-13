// SPDX-FileCopyrightText: 2021 - 2026 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

import QtQuick
import QtQuick.Templates as T
import org.deepin.dtk 1.0 as D
import org.deepin.dtk.style 1.0 as DS

T.SpinBox {
    id: control
    // alert control properties
    property alias alertText: panel.alertText
    property alias alertDuration: panel.alertDuration
    property alias showAlert: panel.showAlert

    implicitWidth: {
        var content = control.implicitContentWidth !== undefined
                ? control.implicitContentWidth
                : (control.contentItem ? control.contentItem.implicitWidth : 0)
        content += 2 * control.padding +
                (up.indicator ? up.indicator.implicitWidth : 0) +
                (down.indicator ? down.indicator.implicitWidth : 0)
        return Math.max(DS.Style.control.backgroundImplicitWidth(control), content)
    }
    implicitHeight:  Math.max(DS.Style.control.implicitHeight(control),
                             up.indicator ? up.indicator.implicitHeight : 0,
                             down.indicator ? down.indicator.implicitHeight : 0)
    baselineOffset: contentItem.y + contentItem.baselineOffset
    padding: 0
    rightPadding: padding + ((down.indicator ? (down.indicator.width + DS.Style.spinBox.spacing)
                                             :  (up.indicator ? (up.indicator.width + DS.Style.spinBox.spacing)
                                                              : 0)))

    validator: IntValidator {
        locale: control.locale.name
        bottom: Math.min(control.from, control.to)
        top: Math.max(control.from, control.to)
    }

    background: EditPanel {
        id: panel
        control: control
        implicitWidth: DS.Style.spinBox.width
        implicitHeight: DS.Style.spinBox.height
    }

    contentItem: TextInput {
        text: control.displayText
        font: control.font
        color: control.palette.text
        selectionColor: control.palette.highlight
        selectedTextColor: control.palette.highlightedText
        horizontalAlignment: Qt.AlignLeft
        verticalAlignment: Qt.AlignVCenter
        leftPadding: DS.Style.spinBox.spacing
        readOnly: !control.editable
        validator: control.validator
        inputMethodHints: control.inputMethodHints
        selectByMouse: control.editable
    }

    // Combined background for both up/down indicators.
    Control {
        id: indicatorContainer
        anchors {
            right: control.right
            rightMargin: DS.Style.spinBox.indicatorSpacing
            verticalCenter: control.verticalCenter
        }
        width: DS.Style.spinBox.indicator.width
        height: control.height - 2 * DS.Style.spinBox.indicatorContainerVMargin
        visible: up.indicator || down.indicator
        z: -1

        property D.Palette indicatorBackground: DS.Style.spinBox.indicatorBackground
        property D.Palette indicatorInnerShadow: DS.Style.spinBox.indicatorInnerShadow

        background: Item {
            Rectangle {
                anchors.fill: parent
                radius: 4
                color: indicatorContainer.D.ColorSelector.indicatorBackground
            }

            Loader {
                active: D.DTK.themeType === D.ApplicationHelper.DarkType
                anchors.fill: parent
                sourceComponent: BoxInsetShadow {
                    shadowBlur: 1
                    shadowOffsetY: -1
                    spread: 0
                    shadowColor: indicatorContainer.D.ColorSelector.indicatorInnerShadow
                    cornerRadius: 4
                }
            }
        }
    }

    up.indicator: SpinBoxIndicator {
        anchors {
            right: control.right
            rightMargin: DS.Style.spinBox.indicatorSpacing
            top: control.top
            topMargin: control.height / 2 - (down.indicator ? height : height / 2)
        }

        direction: SpinBoxIndicator.IndicatorDirection.UpIndicator
        singleIndicator: down.indicator
        pressed: up.pressed
        spinBox: control
    }

    down.indicator: SpinBoxIndicator {
        anchors {
            right: control.right
            rightMargin: DS.Style.spinBox.indicatorSpacing
            bottom: control.bottom
            bottomMargin: control.height / 2 - (up.indicator ? height : height / 2)
        }

        direction: SpinBoxIndicator.IndicatorDirection.DownIndicator
        singleIndicator: up.indicator
        pressed: down.pressed
        spinBox: control
    }
}
