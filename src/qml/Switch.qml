/*
 * Copyright (C) 2022 UnionTech Technology Co., Ltd.
 *
 * Author:     xiaoyaobing <xiaoyaobing@uniontech.com>
 *
 * Maintainer: xiaoyaobing <xiaoyaobing@uniontech.com>
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
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

import QtQuick 2.11
import QtQuick.Templates 2.4 as T
import org.deepin.dtk.impl 1.0 as D
import org.deepin.dtk.style 1.0 as DS

T.Switch {
    id: control

    property D.Palette backgroundColor: DS.Style.switchBackground
    property D.Palette handleColor: DS.Style.switchHandle

    implicitWidth: DS.Style.control.implicitWidth(control)
    implicitHeight: DS.Style.control.implicitHeight(control)

    baselineOffset: contentItem.y + contentItem.baselineOffset
    topPadding: DS.Style.control.vPadding
    bottomPadding: DS.Style.control.vPadding
    spacing: DS.Style.control.spacing

    indicator: Rectangle {
        implicitWidth: DS.Style.switchButton.indicatorWidth
        implicitHeight: DS.Style.switchButton.indicatorHeight

        x: text ? (control.mirrored ? control.width - width - control.rightPadding : control.leftPadding) : control.leftPadding + (control.availableWidth - width) / 2
        y: control.topPadding + (control.availableHeight - height) / 2
        radius: DS.Style.control.radius
        color: control.D.ColorSelector.backgroundColor
        opacity: control.D.ColorSelector.controlState === D.DTK.DisabledState ? 0.4 : 1

        D.DciIcon {
            id: handle
            x: Math.max(0, Math.min(parent.width - width, control.visualPosition * parent.width - (width / 2)))
            y: (parent.height - height) / 2
            sourceSize.width: DS.Style.switchButton.handleWidth
            sourceSize.height: DS.Style.switchButton.handleHeight
            name: DS.Style.switchButton.iconNmae
            opacity: control.D.ColorSelector.controlState === D.DTK.DisabledState ? (control.checked ? 0.4 : 1) : 1
            palette {
                highlight: control.checked ? control.palette.highlight : control.D.ColorSelector.handleColor
                highlightForeground: control.palette.highlightedText
                foreground: control.palette.windowText
                background: control.palette.window
            }
            mode: control.D.ColorSelector.controlState
            theme: control.D.ColorSelector.controlTheme
            fallbackToQIcon: false

            Behavior on x {
                enabled: !control.down
                SmoothedAnimation { velocity: 200 }
            }
        }
    }

    contentItem: Label {
        leftPadding: control.indicator && !control.mirrored ? control.indicator.width + control.spacing : 0
        rightPadding: control.indicator && control.mirrored ? control.indicator.width + control.spacing : 0

        text: control.text
        verticalAlignment: Text.AlignVCenter
    }
}
