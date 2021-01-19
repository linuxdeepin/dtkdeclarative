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
import QtQuick.Templates 2.4 as T
import "PixelMetric.js" as PM

T.SpinBox {
    id: control

    implicitWidth: Math.max(background ? background.implicitWidth : 0,
                            contentItem.implicitWidth + 2 * padding +
                            (up.indicator ? up.indicator.implicitWidth + PM.ControlMargin : 0) +
                            (down.indicator ? down.indicator.implicitWidth + PM.ControlMargin : 0))
    implicitHeight: Math.max(contentItem.implicitHeight + topPadding + bottomPadding,
                             background ? background.implicitHeight : 0,
                             up.indicator ? up.indicator.implicitHeight : 0,
                             down.indicator ? down.indicator.implicitHeight : 0)
    baselineOffset: contentItem.y + contentItem.baselineOffset

    padding: 0
    leftPadding: padding + (!control.mirrored ?
                                (0) :
                                ((down.indicator ? (down.indicator.width + PM.ControlMargin) : 0) + (up.indicator ? (up.indicator.width + PM.ControlMargin) : 0)))
    rightPadding: padding + (control.mirrored ?
                                 (0) :
                                 ((up.indicator ? (PM.ControlMargin + up.indicator.width) : 0) + (down.indicator ? (PM.ControlMargin + down.indicator.width) : 0)))

    validator: IntValidator {
        locale: control.locale.name
        bottom: Math.min(control.from, control.to)
        top: Math.max(control.from, control.to)
    }

    // 控件从左往右的元素为：contentItem + margin + up + margin + down
    // mirrored 模式为：down + margin + up + margin + contentItem
    contentItem: Rectangle {
        implicitWidth: PM.SpinBox_Content_Width
        radius: PM.ControlRadius
        color: control.palette.button

        DRectangleBorder {
            visible: control.activeFocus
            border.color: control.palette.highlight
        }

        TextInput {
            z: 2
            anchors.centerIn: parent

            text: control.displayText
            font: control.font
            color: control.palette.text
            selectionColor: control.palette.highlight
            selectedTextColor: control.palette.highlightedText
            horizontalAlignment: Qt.AlignHCenter
            verticalAlignment: Qt.AlignVCenter

            readOnly: !control.editable
            validator: control.validator
            inputMethodHints: control.inputMethodHints
        }
    }

    up.indicator: Rectangle {
        x: control.mirrored ?
               (down.indicator ? down.indicator.width + PM.ControlMargin : 0) :
               (parent.width - (down.indicator ? down.indicator.width + PM.ControlMargin : 0) - up.indicator.width)

        height: parent.height
        implicitWidth: PM.SpinBox_Height
        implicitHeight: PM.SpinBox_Height
        radius: PM.ControlRadius
        color: up.pressed ? control.palette.mid : control.palette.button

        // 绘制 "+" 号，线宽为 1 个像素
        Rectangle {
            anchors.centerIn: parent
            width: parent.width / 3
            height: 1
            color: enabled && up.pressed ? control.palette.highlight : control.palette.buttonText
        }
        Rectangle {
            anchors.centerIn: parent
            width: 1
            height: parent.width / 3
            color: enabled && up.pressed ? control.palette.highlight : control.palette.buttonText
        }
    }

    down.indicator: Rectangle {
        x: control.mirrored ?
               (0) :
               (parent.width - down.indicator.width)

        height: parent.height
        implicitWidth: PM.SpinBox_Height
        implicitHeight: PM.SpinBox_Height
        radius: PM.ControlRadius
        color: down.pressed ? control.palette.mid : control.palette.button

        // 绘制 "-" 号，线宽为 1 个像素
        Rectangle {
            anchors.centerIn: parent
            width: parent.width / 3
            height: 1
            color: enabled && down.pressed ? control.palette.highlight : control.palette.buttonText
        }
    }
}
