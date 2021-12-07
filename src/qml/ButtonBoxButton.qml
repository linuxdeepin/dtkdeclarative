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
import org.deepin.dtk 1.0 as D
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
    property int position: D.ButtonBoxButton.Position.OnlyOne
    property color color : control.palette.button

    contentItem: IconLabel {
        spacing: control.spacing
        mirrored: control.mirrored
        display: control.display

        icon: control.icon
        text: control.text
        font: control.font
        color: control.palette.highlightedText
    }

    background: Item {
        implicitWidth: control.text.length ? PM.Button_MiniSize + (4 * PM.ControlRadius) : PM.Button_MiniSize + (2 * PM.ControlRadius)
        implicitHeight: PM.Button_MiniSize

        Rectangle {
            id: backgroundRect
            anchors.fill: parent
            radius: position === D.ButtonBoxButton.Position.Middle ? 0 : PM.ControlRadius

            Rectangle {
                id: cliped
                width: position === D.ButtonBoxButton.Position.OnlyOne ? 0 : parent.radius
                height: position === D.ButtonBoxButton.Position.OnlyOne ? 0 : parent.height
                x: position === D.ButtonBoxButton.Position.Beginning ? parent.width - cliped.width : parent.x
                y: parent.y
                color: parent.color
            }

            color: {
                // TODO(Chen Bin): Use a c++ or js function instead.
                if (flat)
                    return "transparent"

                var buttonColor = initGradTopColor
                if (highlighted)
                    buttonColor = palette.highlight

                if (checked) {
                    buttonColor = palette.highlight

                    if (down) {
                        buttonColor = D.DTK.adjustColor(buttonColor, 0, 0, -10, 0, 0, 0, 0)
                    }

                    if (hovered) {
                        buttonColor = D.DTK.adjustColor(buttonColor, 0, 0, +20, 0, 0, 0, 0);
                    }
                } else {
                    if (down) {
                        var hightColor = palette.highlight;
                        hightColor.a = 0.1;

                        buttonColor = D.DTK.adjustColor(buttonColor, 0, 0, -20, 0, 0, +20, 0)
                        buttonColor = D.DTK.blendColor(buttonColor, hightColor)
                    }

                    if (hovered) {
                        buttonColor = D.DTK.adjustColor(buttonColor, 0, 0, -10, 0, 0, 0, 0)
                    }
                }

                return buttonColor
            }
        }

        DropShadow {
            anchors.fill: backgroundRect
            horizontalOffset: 0
            verticalOffset: 4
            radius: 4
            samples: 9
            color: palette.shadow
            source: backgroundRect
        }
    }

    D.Icon {
        id: icon
        anchors.centerIn: parent
        name: iconName
    }
}
