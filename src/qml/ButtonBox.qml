/*
 * Copyright (C) 2021 ~ 2022 UnionTech Software Technology Co., Ltd.
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
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

import QtQuick 2.11

Row {
    id: control

    ButtonGroup {
        id: group
        buttons: __buttons
    }

    default property alias __buttons: control.children

    onPositioningComplete: {
        __updateButtonsPosition()
    }

    // TODO js code
    function __updateButtonsPosition() {
        for (var i = 0; i < group.buttons.length; ++i)
        {
            if (group.buttons.length === 1) {
                group.buttons[i].position = ButtonBoxButton.Position.OnlyOne
            } else if (i == 0) {
                group.buttons[i].position = ButtonBoxButton.Position.Beginning
            } else if (i == group.buttons.length - 1) {
                group.buttons[i].position = ButtonBoxButton.Position.End
            } else {
                group.buttons[i].position = ButtonBoxButton.Position.Middle
            }
        }
    }
}
