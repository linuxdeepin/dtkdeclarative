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
    DSearchEdit 控件：
    等同于 dtkwidget 里边的 DSearchEdit 控件。
*/

FocusScope {
    id: control

    // 暴露给外部的属性
    property alias placeholder: centerIndicatorLabel.text
    property alias placeholderText: content.placeholderText

    implicitWidth: content.implicitWidth
    implicitHeight: content.implicitHeight

    DControlBackground {
        anchors.fill: parent
        color: content.palette.button
        focusBorder.color: content.palette.highlight
        focusBorderVisible: content.activeFocus
    }

    // 输入提示框
    DLineEdit {
        id: content

        readonly property int tfOffset: 8

        width: control.width - searchIcon.width
        height: control.height
        anchors { left: searchIcon.right; leftMargin: -8 }
        focusVisible: false
        focus: true
        visible: content.activeFocus

//        background: Item {}
    }

    // 搜索框中心处图标和字符提示信息，聚焦后隐藏
    Item {
        id: centerIndicator

        anchors.fill: parent
        visible: !content.activeFocus

            Label {
                id: centerIndicatorLabel

                x: control.width / 2
                anchors.verticalCenter: parent.verticalCenter
            }

        MouseArea {
            anchors.fill: parent
            onClicked: {
                content.forceActiveFocus(Qt.MouseFocusReason)
            }
        }
    }

    // 聚焦后左侧的搜索图标
    DIcon {
        id: searchIcon

        x: !control.activeFocus? (control.width / 2) - width : 0
        anchors.verticalCenter: parent.verticalCenter
        name: "search_action"
    }
}
