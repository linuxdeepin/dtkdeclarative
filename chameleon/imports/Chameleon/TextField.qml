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
import org.deepin.dtk 1.0 as D
import "PixelMetric.js" as PM

T.TextField {
    id: control

    implicitWidth: Math.max(background ? background.implicitWidth : 0,
                            placeholderText ? placeholder.implicitWidth + leftPadding + rightPadding : 0)
                   || contentWidth + leftPadding + rightPadding
    implicitHeight: Math.max(contentHeight + topPadding + bottomPadding,
                             background ? background.implicitHeight : 0,
                             placeholder.implicitHeight + topPadding + bottomPadding)

    padding: PM.ControlPadding

    color: control.palette.text
    selectionColor: control.palette.highlight
    selectedTextColor: control.palette.highlightedText
    verticalAlignment: TextInput.AlignVCenter

    PlaceholderText {
        id: placeholder
        x: control.leftPadding
        y: control.topPadding
        width: control.width - (control.leftPadding + control.rightPadding)
        height: control.height - (control.topPadding + control.bottomPadding)

        text: control.placeholderText
        font: control.font
        color: palette.placeholderText
        verticalAlignment: control.verticalAlignment
        visible: !control.length && !control.preeditText && (!control.activeFocus || control.horizontalAlignment !== Qt.AlignHCenter)
        renderType: control.renderType
    }

    background: Rectangle {
        implicitWidth: PM.TextField_width
        implicitHeight: PM.TextField_Height
        radius: PM.ControlRadius
        color: control.palette.button

       D.RectangleBorder {
            visible: control.activeFocus
            border.color: control.palette.highlight
        }
    }
}
