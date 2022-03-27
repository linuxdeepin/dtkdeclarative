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

import QtQuick 2.0
import QtQuick.Controls 2.4

Item {
    id: control
    property alias source: image.source
    property real imageScale: 1
    signal clicked
    default property alias content: loader.sourceComponent
    implicitWidth: childrenRect.width
    implicitHeight: childrenRect.height

    function trigger() {
        if (loader.item) {
            if (loader.item.hasOwnProperty("popup")) {
                loader.item.popup(control)
            } else if (loader.item.hasOwnProperty("open")) {
                loader.item.open()
            } else if (loader.item.hasOwnProperty("show")) {
                loader.item.show()
            }
        }
        control.clicked()
    }

    Image {
        id: image
        width: sourceSize.width * imageScale
        height: sourceSize.height * imageScale
        MouseArea {
            anchors.fill: parent
            onClicked: trigger()
        }
        Loader {
            id: loader
        }
    }
}
