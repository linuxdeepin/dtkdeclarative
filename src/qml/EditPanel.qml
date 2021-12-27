/*
 * Copyright (C) 2020 ~ 2021 UnionTech Technology Co., Ltd.
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

import QtQuick 2.11
import org.deepin.dtk.impl 1.0 as DI
import org.deepin.dtk.style 1.0 as DS


Rectangle {
    id: panel
    property Item control
    radius: DS.Style.control.radius
    color: colorSelector.palettes ? colorSelector.editBackground
                                  : "transparent"

    DI.ColorSelector {
        id: colorSelector
        control: panel.control

        palettes: [
            DS.Style.editBackground
        ]
    }

    RectangleBorder {
        visible: control.activeFocus
        borderColor: control.palette.highlight
    }
}
