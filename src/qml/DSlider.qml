/*
 * Copyright (C) 2020 ~ 2021 Uniontech Software Technology Co.,Ltd.
 *
 * Author:     xiaoyaobing <xiaoyaobing@uniontech.com>
 *
 * Maintainer: xiaoyaobing <xiaoyaobing@uniontech.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

import QtQuick 2.0
import QtQuick.Controls 2.4
import QtQuick.Shapes 1.11

Slider {
    id: control

    /*! 滑块宽度 */
    property real handelWidth
    /*! 滑块高度 */
    property real handelHeight
    /*! 滑块圆弧半径 */
    property real handelRadius
    /*! 滑槽已滑区域颜色和滑块颜色 */
    property alias activeColor: sliderHandle.color
    /*! 滑槽背景色 */
    property alias grooveColor: dashLine.strokeColor
    /*! 滑槽尺寸，当Slider水平模式代表高度，竖直模式代表宽度 */
    property real grooveSize

    // 绘制背景
    background: Shape {
        anchors.fill: parent

        // 绘制总区间刻度线
        ShapePath {
            id: dashLine

            capStyle: ShapePath.FlatCap
            strokeStyle: ShapePath.DashLine
            strokeColor: control.grooveColor
            strokeWidth: control.grooveSize
            dashOffset: 0
            dashPattern: [0.5, 0.25]
            startX: control.horizontal ? 0 : control.width / 2
            startY : control.horizontal ? control.height / 2 : 0
            PathLine {
                x: control.horizontal ? control.width : control.width / 2
                y: control.horizontal ? control.height / 2 : control.height
            }
        }

        // 绘制滑槽到起点区间刻度线
        ShapePath {
            capStyle: ShapePath.FlatCap
            strokeStyle: ShapePath.DashLine
            strokeColor: sliderHandle.color
            strokeWidth: control.grooveSize
            dashOffset: 0
            dashPattern: [0.5, 0.25]
            startX: control.horizontal ? 0 : control.width / 2
            startY : control.horizontal ? control.height / 2 : 0
            PathLine {
                x: control.horizontal ? control.value * control.width: control.width / 2
                y: control.horizontal ? control.height / 2 : control.visualPosition * control.height
            }
        }
    }

    // 自绘滑块
    handle: Rectangle {
        id: sliderHandle

        x: control.horizontal ? control.visualPosition * (control.width - control.handelWidth) : (control.width - control.handelWidth) / 2
        y: control.horizontal ? (control.height - control.handelHeight) / 2 : control.visualPosition * (control.height - control.handelHeight)
        width: control.handelWidth
        height: control.handelHeight
        radius: control.handelRadius
        color: control.handleColor
    }
}
