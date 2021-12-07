/*
 * Copyright (C) 2020 ~ 2020 Deepin Technology Co., Ltd.
 *
 * Author:     liuyang <liuyang@uniontech.com>
 *
 * Maintainer: liuyang <liuyang@uniontech.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

import QtQuick 2.11
import QtQuick.Controls 2.4
import QtQuick.Controls.impl 2.4
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
