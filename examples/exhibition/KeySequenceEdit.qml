/*
 * Copyright (C) 2022 UnionTech Technology Co., Ltd.
 *
 * Author:     Chen Bin <chenbin@uniontech.com>
 *
 * Maintainer: Chen Bin <chenbin@uniontech.com>
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

import QtQuick 2.0
// 确保dtk的模块在最后被引入
import org.deepin.dtk 1.0

Column {
    Label {
        width: parent.width
        wrapMode: Text.WordWrap
        text: "应用设置和控制中心部分设置快捷键的地方。"
        horizontalAlignment: Qt.AlignHCenter
    }

    Flow {
        spacing: 10
        width: parent.width

        KeySequenceEdit {
            width: 504
            height: 36
            text: "切换键盘布局"
            placeholderText: "请重新输入快捷键"
            keys: ["CTRL", "SHIFT"]
        }

        KeySequenceEdit {
            width: 504
            height: 36
            text: "关闭窗口"
            placeholderText: "请重新输入快捷键"
            keys: ["ALT", "F4"]
        }

        KeySequenceEdit {
            width: 504
            height: 36
            text: "关闭窗口"
            placeholderText: "请重新输入快捷键"
        }
    }
}
