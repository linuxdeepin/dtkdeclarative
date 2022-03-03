/*
 * Copyright (C) 2020 ~ 2020 Deepin Technology Co., Ltd.
 *
 * Author:     sunkang <sunkang@uniontech.com>
 *
 * Maintainer: sunkang <sunkang@uniontech.com>
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
import QtQuick.Templates 2.4 as T
import QtQuick.Controls 2.4
import QtQuick.Controls.impl 2.4
import org.deepin.dtk 1.0 as D
import "PixelMetric.js" as PM

T.RoundButton {
    id: control

    implicitWidth: Math.max(background ? background.implicitWidth : 0,
                            contentItem.implicitWidth + leftPadding + rightPadding)
    implicitHeight: Math.max(background ? background.implicitHeight : 0,
                             contentItem.implicitHeight + topPadding + bottomPadding)
    baselineOffset: contentItem.y + contentItem.baselineOffset

    D.DciIcon.mode: D.ColorSelector.controlState
    D.DciIcon.theme: D.ColorSelector.controlTheme
    D.DciIcon.palette: D.DTK.makeIconPalette(palette)
    padding: PM.ControlPadding
    spacing: PM.ControlSpacing

    icon.width: 16
    icon.height: 16

    width: PM.RoundButton_ImplicitWidth
    height: PM.RoundButton_ImplicitHeight

    contentItem: D.IconLabel {
        spacing: control.spacing
        mirrored: control.mirrored
        display: control.display

        icon: D.DTK.makeIcon(control.icon, control.D.DciIcon)
        text: control.text
        font: control.font
        color: control.palette.buttonText
    }

    background: Rectangle {
        implicitWidth: parent.width
        implicitHeight: parent.height
        visible: !control.flat || control.down || control.checked

        gradient: Gradient {
            GradientStop {
                position: 0
                color: control.down || control.checked ? Qt.lighter(control.palette.highlight,0.8) : control.palette.highlight
            }

            GradientStop {
                position: 1
                color: control.down || control.checked ? Qt.lighter(control.palette.highlight,0.8) : control.palette.highlight
            }
        }

        radius: control.radius
        border.color: control.down || control.checked ? Qt.lighter(control.palette.highlight,0.8) : control.palette.highlight

        Rectangle {
            x: -(2 * PM.ControlFocusBorderWidth)
            y: -(2 * PM.ControlFocusBorderWidth)
            width: parent.width + (2 * (PM.ControlFocusBorderWidth + PM.ControlFocusBorderWidth))
            height: parent.height + (2 * (PM.ControlFocusBorderWidth + PM.ControlFocusBorderWidth))
            radius: width / 2
            color: "transparent"
            border.width: PM.ControlFocusBorderWidth
            border.color: control.palette.highlight
            visible: control.focus
        }
    }
}
