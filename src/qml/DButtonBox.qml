/*
 * Copyright (C) 2021 ~ 2021 UnionTech Software Technology Co., Ltd.
 *
 * Author:     wangfei <wangfeia@uniontech.com>
 *
 * Maintainer: wangfei <wangfeia@uniontech.com>
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
import QtQuick.Layouts 1.11
import com.deepin.dtk 1.0

/*
    DButtonBox 控件：
    等同于 dtkwidget 里边的 DButtonBox 控件。
*/
Rectangle {
    id: control

    ButtonGroup {
        id: group
        buttons: control.children[0].children
    }

    Component.onCompleted: {
        internal.updateButtonsPosition()
    }

    QtObject {
        id: internal
        function updateButtonsPosition() {
            for (var i = 0; i < group.buttons.length; ++i)
            {
                if (group.buttons.length === 1) {
                    group.buttons[i].position = DButtonBoxButton.Position.OnlyOne
                } else if (i == 0) {
                    group.buttons[i].position = DButtonBoxButton.Position.Beginning
                } else if (i == group.buttons.length - 1) {
                    group.buttons[i].position = DButtonBoxButton.Position.End
                } else {
                    group.buttons[i].position = DButtonBoxButton.Position.Middle
                }
            }
        }
    }
}
