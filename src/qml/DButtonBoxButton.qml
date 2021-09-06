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
import com.deepin.dtk 1.0
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
    property int position: DButtonBoxButton.Position.OnlyOne
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

    background: Rectangle {
        id: backgroundRect
        implicitWidth: control.text.length ? PM.Button_MiniSize + (4 * PM.ControlRadius) : PM.Button_MiniSize + (2 * PM.ControlRadius)
        implicitHeight: PM.Button_MiniSize
        radius: position == DButtonBoxButton.Position.Middle ? 0 : PM.ControlRadius
        color: control.palette.button
        Rectangle {
            id: cliped
            width: position == DButtonBoxButton.Position.OnlyOne ? 0 : parent.radius
            height: position == DButtonBoxButton.Position.OnlyOne ? 0 : parent.height
            x: position == DButtonBoxButton.Position.Beginning ? parent.width - cliped.width : parent.x
            y: parent.y
            color: parent.color
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

    MouseArea{
        id: mouseArea
        anchors.fill: parent;
        hoverEnabled: true;
        onPressed: {
            var hightColor = palette.highlight;
            hightColor.a = 0.1;

            color = checked ? palette.highlight : palette.button

            color = DTK.adjustColor(color, 0, 0, -20, 0, 0, +20, 0);
            color = DTK.blendColor(color, hightColor);

            backgroundRect.color = color
        }
        onEntered: {
            color = checked ? palette.highlight : palette.button
            color = DTK.adjustColor(color, 0, 0, -10, 0, 0, 0, 0);
            backgroundRect.color = color
        }
        onExited: {
            backgroundRect.color = Qt.binding(function(){return checked ? palette.highlight : palette.button})
        }
        onReleased: {
            color = checked ? palette.highlight : palette.button
            color= DTK.adjustColor(color, 0, 0, -10, 0, 0, 0, 0);
            backgroundRect.color = color
            control.released()
            checked = true
        }
        onClicked: {
            control.clicked();
        }
    }

    DIcon {
        id: icon
        anchors.centerIn: parent
        name: iconName
    }

    onCheckedChanged: {
        color = checked ? palette.highlight : palette.button
        mouseArea.onExited();
    }
}
