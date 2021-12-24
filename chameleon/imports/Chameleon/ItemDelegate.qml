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
import org.deepin.dtk 1.0 as D
import "PixelMetric.js" as PM

T.ItemDelegate {
    id: control

    implicitWidth: Math.max(background ? background.implicitWidth : 0,
                            contentItem.implicitWidth + leftPadding + rightPadding)
    implicitHeight: Math.max(background ? background.implicitHeight : 0,
                             Math.max(contentItem.implicitHeight,
                                      indicator ? indicator.implicitHeight : 0) + topPadding + bottomPadding)
    baselineOffset: contentItem.y + contentItem.baselineOffset

    padding: PM.ControlPadding
    spacing: PM.ControlSpacing

    contentItem: Item {
        D.QtIcon {
            id: mark_indicator
            x: PM.ControlRadius
            y: (parent.height - height) / 2
            name: "mark_indicator"
            color: control.hovered ? control.palette.base : control.palette.text
            visible: checked
        }

        D.IconLabel {
            x: mark_indicator.width + (2 * PM.ControlRadius)
            spacing: control.spacing
            mirrored: control.mirrored
            display: control.display
            alignment: control.display === IconLabel.IconOnly || control.display === IconLabel.TextUnderIcon ? Qt.AlignCenter : Qt.AlignLeft
            text: control.text
            font: control.font
            color: control.hovered ? control.palette.base : control.palette.text
            icon: D.DTK.makeIcon(control.icon, control.D.DciIcon)
        }
    }

    background: Rectangle {
        implicitHeight: PM.ItemDelegate_Height

        color: control.hovered ? control.palette.highlight : "transparent"
    }
}
