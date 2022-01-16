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

import QtQuick 2.11
import org.deepin.dtk.style 1.0 as DS
import ".."

Item {
    id: control
    property Item progressBar

    Loader {
        anchors.fill: parent
        sourceComponent: progressBar.formatText ? _textPanelComponent : _normalTextComponent
    }

    Component {
        id: _textPanelComponent
        BoxPanel {}
    }

    Component {
        id: _normalTextComponent
        Rectangle {
            radius: DS.Style.control.radius
            color: Qt.rgba(0, 0, 0, 0.1);
        }
    }
}
