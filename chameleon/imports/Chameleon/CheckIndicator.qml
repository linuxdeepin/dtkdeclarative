/*
 * Copyright (C) 2020 ~ 2020 Uniontech Technology Co., Ltd.
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
import org.deepin.dtk 1.0 as D
import "PixelMetric.js" as PM

Rectangle {
    id: indicatorItem

    property Item control
    property int checkState: control.checkState

    implicitWidth: PM.CheckBox_Indicator_FocusWidth
    implicitHeight: PM.CheckBox_Indicator_FocusHeight
    border.color: control.focus ? control.palette.highlight : "transparent"
    border.width: PM.CheckBox_Indicator_FocusBorderWidth
    radius: PM.CheckBox_Indicator_FocusRadius
    color: "transparent"

    D.Icon {
        name: "checked"
        anchors.centerIn: parent
        width: PM.CheckBox_Indicator_CheckedWidth
        height: PM.CheckBox_Indicator_CheckedHeight
        color: control.palette.highlight
        visible: indicatorItem.checkState === Qt.Checked
    }

    Rectangle {
        anchors.centerIn: parent
        width: PM.CheckBox_Indicator_UncheckedHeight
        height: PM.CheckBox_Indicator_UncheckedHeight
        radius: PM.CheckBox_Indicator_UncheckedRadius
        antialiasing: true
        border.color: control.palette.windowText
        border.width: PM.CheckBox_Indicator_UncheckedBorderWidth
        visible: indicatorItem.checkState !== Qt.Checked

        Rectangle {
            anchors.centerIn: parent
            width: parent.width / 2
            height: PM.CheckBox_Indicator_PartiallyCheckedHeight
            color: control.palette.windowText
            visible: indicatorItem.checkState === Qt.PartiallyChecked
        }
    }
}
