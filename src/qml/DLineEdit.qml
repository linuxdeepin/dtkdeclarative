/*
 * Copyright (C) 2020 ~ 2021 Deepin Technology Co., Ltd.
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
import com.deepin.dtk 1.0

/*
    DLineEdit 控件：
    等同于 dtkwidget 里边的 DLineEdit 控件。
*/

FocusScope {
    id: control

    // 暴露给外部的属性
    property bool focusVisible: true
    // 以后 Item 可能会修改为 TextField，所以导出 TextField 的属性
    property alias placeholderText: content.placeholderText
    property alias background: content.background
    property alias focusReason: content.focusReason
    property alias hovered : content.hovered
    property alias palette: content.palette

    implicitWidth: content.implicitWidth
    implicitHeight: content.implicitHeight

    DControlBackground {
        anchors.fill: parent
        color: content.palette.button
        focusBorder.color: content.palette.highlight
        focusBorderVisible: control.focusVisible && content.activeFocus
    }

    TextField {
        id: content

        width: control.width - clearBtn.width
        height: control.height
        focus: true
        background: Item {
            implicitWidth: 180
            implicitHeight: 36
        }
    }

    Button {
        id: clearBtn

        width: visible ? implicitWidth : 0
        anchors { left: content.right; verticalCenter: parent.verticalCenter }
        icon.name: "window-close_round"
        visible: (content.activeFocus) && (content.text.length !== 0)
        onClicked: {
            content.clear()
        }
    }
}
