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
import QtQuick.Controls 2.4
import org.deepin.dtk 1.0 as D

Control {
    id: control
    hoverEnabled: true
    property D.Palette backgroundColor: D.Palette {
        normal: Qt.hsla(Math.random(), 0.2, 0.5, 0.9)
        hovered: Qt.hsla(Math.random(), 0.1, 0.1, 0.9)
    }
    property D.Palette textColor: D.Palette {
        normal: Qt.hsla(Math.random(), 0.5, 0.2, 0.4)
        hovered: Qt.hsla(Math.random(), 0.6, 0.6, 0.9)
    }

    property string text

    background: Rectangle {
        color: control.D.ColorSelector.backgroundColor
        Behavior on color {
            ColorAnimation {
                duration: 300
            }
        }
    }

    contentItem: Text {
        id: textItem
        anchors.fill: parent
        horizontalAlignment: Qt.AlignHCenter
        verticalAlignment: Qt.AlignVCenter
        text: control.text
        color: control.D.ColorSelector.textColor

        Behavior on color {
            ColorAnimation {
                duration: 300
            }
        }
    }
}
