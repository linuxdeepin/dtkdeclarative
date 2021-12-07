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
import QtGraphicalEffects 1.0
import "PixelMetric.js" as PM

ToolButton {
    id: control

    implicitWidth: 36
    implicitHeight: 36

    background: Item {
        implicitWidth: control.text.length ? PM.Button_MiniSize + (4 * PM.ControlRadius) : PM.Button_MiniSize + (2 * PM.ControlRadius)
        implicitHeight: PM.Button_MiniSize

        Rectangle {
            id: rect
            anchors.fill: parent
            radius: PM.ControlRadius
            color: {
                var backgroundColor = "transparent"

                if (flat)
                    return backgroundColor

                if (hovered) {
                    backgroundColor = Qt.rgba(0, 0, 0, 0.05)
                }

                if (down || highlighted || checked) {
                    backgroundColor = Qt.rgba(0, 0, 0, 0.1)
                }

                return backgroundColor
            }
        }

        DropShadow {
            anchors.fill: rect
            horizontalOffset: 0
            verticalOffset: 2
            radius: 4
            samples: 9
            color: palette.shadow
            source: rect
        }
    }
}

