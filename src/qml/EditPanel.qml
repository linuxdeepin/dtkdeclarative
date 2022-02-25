/*
 * Copyright (C) 2021 ~ 2022 UnionTech Technology Co., Ltd.
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
import org.deepin.dtk.impl 1.0 as D
import org.deepin.dtk.style 1.0 as DS


Rectangle {
    id: panel
    property Item control
    property D.Palette backgroundColor: DS.Style.editBackground
    property D.Palette alertBackgroundColor: DS.Style.alertBackground
    property alias alertText: _alert.text
    property alias alertDuration: _alert.timeout
    property alias showAlert: _alert.visible
    property alias showBorder: _border.visible

    radius: DS.Style.control.radius
    color: showAlert ? D.ColorSelector.alertBackgroundColor
                     : D.ColorSelector.backgroundColor

    FocusBoxBorder {
        id: _border
        anchors.fill: parent
        visible: control.activeFocus
        color: control.palette.highlight
        radius: parent.radius
    }

    AlertToolTip {
        id: _alert
        target: control
    }
}
