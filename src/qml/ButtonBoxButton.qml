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
import QtGraphicalEffects 1.0
import org.deepin.dtk.impl 1.0 as D
import "PixelMetric.js" as PM

Button {
    id: control
    checkable: true
    enum Position {
        Beginning,
        Middle,
        End,
        OnlyOne
    }
    property int position: ButtonBoxButton.Position.OnlyOne
    property color color : control.palette.button

    background: Item {
        implicitWidth: control.text.length ? PM.Button_MiniSize + (4 * PM.ControlRadius) : PM.Button_MiniSize + (2 * PM.ControlRadius)
        implicitHeight: PM.Button_MiniSize
        DropShadow {
            anchors.fill: backgroundRect
            horizontalOffset: 0
            verticalOffset: 4
            radius: 4
            samples: 9
            color: palette.shadow
            source: backgroundRect
        }

        Rectangle {
            id: backgroundRect
            anchors.fill: parent
            radius: position === ButtonBoxButton.Position.Middle ? 0 : PM.ControlRadius
            //TODO(Chen Bin): add checked state color

            Rectangle {
                id: cliped
                width: position === ButtonBoxButton.Position.OnlyOne ? 0 : parent.radius
                height: position === ButtonBoxButton.Position.OnlyOne ? 0 : parent.height
                x: position === ButtonBoxButton.Position.Beginning ? parent.width - cliped.width : parent.x
                y: parent.y
                color: parent.color
            }
        }
    }
}
