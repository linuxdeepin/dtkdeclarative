/*
 * Copyright (C) 2020 ~ 2022 Deepin Technology Co., Ltd.
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

/*
    ControlBackground 控件：
    可以用作组合控件 button、edit 等的背景颜色以及焦点框的显示。
*/

Rectangle {
    id: control

    // 暴露给外部的属性
    property int focusBorderSpace: 1
    property alias focusBorder: contentBorder.border
    property alias focusBorderVisible: contentBorder.visible

    radius: 8
    color: "black"

    Rectangle {
        id: contentBorder

        anchors.centerIn: parent
        width: parent.width + 2 * (control.focusBorderSpace + border.width)
        height: parent.height + 2 * (control.focusBorderSpace + border.width)
        radius: parent.radius + control.focusBorderSpace + border.width
        border { width: 2; color: "black" }
        color: "transparent"
    }
}
