/*
 * Copyright (C) 2022 UnionTech Technology Co., Ltd.
 *
 * Author:     yeshanshan <yeshanshan@uniontech.com>
 *
 * Maintainer: yeshanshan <yeshanshan@uniontech.com>
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
import org.deepin.dtk.impl 1.0 as D
import org.deepin.dtk.style 1.0 as DS

Rectangle {
    property D.Palette backgroundColor: DS.Style.buttonIndicator.background
    property Control control: D.ColorSelector.control

    implicitWidth: DS.Style.buttonIndicator.size
    implicitHeight: implicitHeight
    radius: width / 2
    color: D.ColorSelector.backgroundColor

    D.DciIcon {
        anchors.centerIn: parent
        sourceSize {
            width: DS.Style.buttonIndicator.iconSize
        }
        name: "arrow_ordinary_down"
        palette: control.D.DTK.makeIconPalette(control.palette)
        mode: control.D.ColorSelector.controlState
        theme: control.D.ColorSelector.controlTheme
        fallbackToQIcon: false
    }
}
