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
import QtQuick.Window 2.11
import QtQuick.Controls 2.4
import QtQuick.Controls.impl 2.4
import QtQuick.Templates 2.4 as T
import "PixelMetric.js" as PM

T.ComboBox {
    id: control

    implicitWidth: Math.max(background ? background.implicitWidth : 0,
                            contentItem.implicitWidth + leftPadding + rightPadding)
    implicitHeight: Math.max(background ? background.implicitHeight : 0,
                             Math.max(contentItem.implicitHeight,
                                      indicator ? indicator.implicitHeight : 0) + topPadding + bottomPadding)
    baselineOffset: contentItem.y + contentItem.baselineOffset

    padding: PM.ControlRadius

    leftPadding: padding + (!control.mirrored || !indicator || !indicator.visible ? 0 : indicator.width + spacing)
    rightPadding: padding + (control.mirrored || !indicator || !indicator.visible ? 0 : indicator.width + spacing)

    delegate: ItemDelegate {
        width: parent.width
        text: control.textRole ? (Array.isArray(control.model) ? modelData[control.textRole] : model[control.textRole]) : modelData
        checked: control.currentIndex === index
        hoverEnabled: control.hoverEnabled
    }

    indicator: ColorImage {
        x: control.mirrored ? control.padding : control.width - width - control.padding
        y: control.topPadding + (control.availableHeight - height) / 2

        defaultColor: control.palette.dark
        color: control.palette.buttonText
        source: "qrc:/assets/platformthemeplugin/icons/texts/combobox_arrow_12px.svg"
    }

    contentItem: T.TextField {
        text: control.editable ? control.editText : control.displayText

        enabled: control.editable
        autoScroll: control.editable
        readOnly: control.down
        inputMethodHints: control.inputMethodHints
        validator: control.validator

        font: control.font
        color: control.editable ? control.palette.text : control.palette.buttonText
        selectionColor: control.palette.highlight
        selectedTextColor: control.palette.highlightedText
        verticalAlignment: Text.AlignVCenter
    }

    background: Rectangle {
        implicitWidth: PM.ComboBox_Width
        implicitHeight: PM.ComboBox_Heigt
        radius: PM.ControlRadius
        color: control.palette.button

        Rectangle {
            x: -(2 * PM.ControlFocusBorderWidth)
            y: -(2 * PM.ControlFocusBorderWidth)
            width: parent.width + (2 * (PM.ControlFocusBorderWidth + PM.ControlFocusBorderWidth))
            height: parent.height + (2 * (PM.ControlFocusBorderWidth + PM.ControlFocusBorderWidth))
            radius: PM.ControlRadius + (PM.ControlFocusBorderWidth + PM.ControlFocusBorderWidth)
            color: "transparent"
            border.width: PM.ControlFocusBorderWidth
            border.color: control.palette.highlight
            visible: control.focus
        }
    }

    popup: T.Popup {
        x: -(2 * PM.ControlFocusBorderWidth)
        y: -(PM.ControlRadius + control.currentIndex * contentItem.implicitHeight / control.count)
        width: control.width + (2 * (PM.ControlFocusBorderWidth + PM.ControlFocusBorderWidth))
        height: Math.min(contentItem.implicitHeight, control.Window.height - topMargin - bottomMargin) + 2 * PM.ControlRadius

        contentItem:ListView {
            clip: true
            implicitHeight: contentHeight
            model: control.delegateModel
            currentIndex: control.highlightedIndex
            highlightMoveDuration: 0
            topMargin: PM.ControlRadius
            bottomMargin: PM.ControlRadius

            T.ScrollBar.vertical: ScrollBar { }
        }

        background: Rectangle {
            width: parent.width
            height: parent.height
            color: control.palette.base
            border.color: control.palette.button
            radius: PM.ControlRadius
        }
    }
}
