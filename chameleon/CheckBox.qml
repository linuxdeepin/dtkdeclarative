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

T.CheckBox {
    id: control

    implicitWidth: Math.max(background ? background.implicitWidth : 0,
                            contentItem.implicitWidth + leftPadding + rightPadding)
    implicitHeight: Math.max(background ? background.implicitHeight : 0,
                             Math.max(contentItem.implicitHeight,
                                      indicator ? indicator.implicitHeight : 0) + topPadding + bottomPadding)
    baselineOffset: contentItem.y + contentItem.baselineOffset

    padding: PM.ControlPadding
    spacing: PM.ControlSpacing

    indicator: Rectangle {
        implicitWidth: PM.CheckBox_Indicator_FocusWidth
        implicitHeight: PM.CheckBox_Indicator_FocusHeight
        x: text ? (control.mirrored ? control.width - width - control.rightPadding : control.leftPadding) : control.leftPadding + (control.availableWidth - width) / 2
        y: control.topPadding + (control.availableHeight - height) / 2
        border.color: control.focus ? control.palette.highlight : "transparent"
        border.width: PM.CheckBox_Indicator_FocusBorderWidth
        radius: PM.CheckBox_Indicator_FocusRadius
        color: "transparent"

        ColorImage {
            x: (parent.width - width) / 2
            y: (parent.height - height) / 2
            width: PM.CheckBox_Indicator_CheckedWidth
            height: PM.CheckBox_Indicator_CheckedHeight
            defaultColor: control.palette.changliangtext
            color: control.checkState === Qt.Checked ? control.palette.highlight : defaultColor
            source: "qrc:/assets/platformthemeplugin/icons/texts/checked_20px.svg"
            visible: control.checkState === Qt.Checked
        }

        Rectangle {
            x: (parent.width - width) / 2
            y: (parent.height - height) / 2
            width: PM.CheckBox_Indicator_UncheckedHeight
            height: PM.CheckBox_Indicator_UncheckedHeight
            radius: PM.CheckBox_Indicator_UncheckedRadius
            antialiasing: true
            border.color: control.palette.windowText
            border.width: PM.CheckBox_Indicator_UncheckedBorderWidth
            visible: control.checkState !== Qt.Checked

            Rectangle {
                x: (parent.width - width) / 2
                y: (parent.height - height) / 2
                width: parent.width / 2
                height: PM.CheckBox_Indicator_PartiallyCheckedHeight
                color: control.palette.windowText
                visible: control.checkState === Qt.PartiallyChecked
            }
        }
    }

    contentItem: Text {
        leftPadding: control.indicator && !control.mirrored ? control.indicator.width + control.spacing : 0
        rightPadding: control.indicator && control.mirrored ? control.indicator.width + control.spacing : 0

        text: control.text
        font: control.font
        color: control.palette.windowText
    }
}

