// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

import QtQuick 2.11
import QtQuick.Templates 2.4 as T
import "PixelMetric.js" as PM

T.RadioButton {
    id: control

    implicitWidth: Math.max(background ? background.implicitWidth : 0,
                            contentItem.implicitWidth + leftPadding + rightPadding)
    implicitHeight: Math.max(background ? background.implicitHeight : 0,
                             Math.max(contentItem.implicitHeight,
                                      indicator ? indicator.implicitHeight : 0) + topPadding + bottomPadding)
    baselineOffset: contentItem.y + contentItem.baselineOffset

    padding: PM.ControlPadding
    spacing: PM.ControlSpacing
    topPadding: padding + PM.ControlMargin
    bottomPadding: padding + PM.ControlMargin

    indicator: Rectangle {
        x: text ? (control.mirrored ? control.width - width - control.rightPadding : control.leftPadding) : control.leftPadding + (control.availableWidth - width) / 2
        y: control.topPadding + (control.availableHeight - height) / 2
        width:  PM.RadioButton_Indicator_FocusWidth
        height: PM.RadioButton_Indicator_FocusHeight
        border.color: control.focus ? control.palette.highlight : "transparent"
        border.width: PM.RadioButton_Indicator_FocusBorderWidth
        radius: height / 2

        Rectangle {
            x: (parent.width - width) / 2
            y: (parent.height - height) / 2
            width: control.checked ? PM.RadioButton_Indicator_CheckedWidth : PM.RadioButton_Indicator_UncheckedWidth
            height: control.checked ? PM.RadioButton_Indicator_CheckedHeight : PM.RadioButton_Indicator_UncheckedHeight
            radius: height / 2
            border.color: control.checked ? control.palette.highlight : control.palette.buttonText
            border.width: control.checked ? (width / 4) : PM.RadioButton_Indicator_UncheckeBorderWidth
        }
    }

    contentItem: Text {
        leftPadding: control.indicator && !control.mirrored ? control.indicator.width + control.spacing : 0
        rightPadding: control.indicator && control.mirrored ? control.indicator.width + control.spacing : 0

        text: control.text
        font: control.font
        color: control.palette.windowText
        verticalAlignment: Text.AlignVCenter
    }

    MouseArea {
        anchors.fill: parent
        onClicked: {
            control.checked = !control.checked;
            mouse.accepted = false
        }
    }
}
