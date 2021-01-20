/*
 * Copyright (C) 2020 ~ 2020 Deepin Technology Co., Ltd.
 *
 * Author:     liuyang <liuyang@uniontech.com>
 *
 * Maintainer: liuyang <liuyang@uniontech.com>
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
import QtQuick.Shapes 1.11
import "PixelMetric.js" as PM

Item {
    id: control

    property color color: "red"
    property int arrowPosition: 0

    enum ArrowPosition {
        NoArrow = 0,
        ArrowAbove = 1,
        ArrowLeft = 2,
        ArrowBelow = 3,
        ArrowRight = 4
    }

    // 多重采样抗锯齿
    layer.enabled: true
    layer.samples: 8

    //平滑处理
    smooth: true

    // 使用抗锯齿
    antialiasing: true

    // 无尖角
    Rectangle {
        visible: SliderHandle.ArrowPosition.NoArrow === control.arrowPosition ? true : false
        x: 0
        y: 0
        width: control.width
        height: control.height
        radius: PM.ControlRadius
        color: control.color
    }

    // 尖角向上
    Shape {
        visible: SliderHandle.ArrowPosition.ArrowAbove === control.arrowPosition ? true : false
        ShapePath {
            strokeColor: control.color
            fillColor: control.color
            fillRule: ShapePath.WindingFill
            startX: control.width / 2
            startY: 0
            PathLine {
                x: control.width
                y: PM.ControlRadius
            }
            PathLine {
                x: control.width
                y: control.height - PM.ControlRadius
            }
            PathArc {
                x: control.width - PM.ControlRadius
                y: control.height
                radiusX: PM.ControlRadius
                radiusY: PM.ControlRadius
            }
            PathLine {
                x: PM.ControlRadius
                y: control.height
            }
            PathArc {
                x: 0
                y: control.height - PM.ControlRadius
                radiusX: PM.ControlRadius
                radiusY: PM.ControlRadius
            }
            PathLine {
                x: 0
                y: PM.ControlRadius
            }
            PathLine {
                x: control.width / 2
                y: 0
            }
        }
    }

    // 尖角向下
    Shape {
        visible: SliderHandle.ArrowPosition.ArrowBelow === control.arrowPosition ? true : false
        ShapePath {
            strokeColor: control.color
            fillColor: control.color
            fillRule: ShapePath.WindingFill
            startX: PM.ControlRadius
            startY: 0
            PathLine {
                x: control.width - PM.ControlRadius
                y: 0
            }
            PathArc {
                x: control.width
                y: PM.ControlRadius
                radiusX: PM.ControlRadius
                radiusY: PM.ControlRadius
            }
            PathLine {
                x: control.width
                y: control.height - PM.ControlRadius
            }
            PathLine {
                x: control.width / 2
                y: control.height
            }
            PathLine {
                x: 0
                y: control.height - PM.ControlRadius
            }
            PathLine {
                x: 0
                y: PM.ControlRadius
            }
            PathArc {
                x: PM.ControlRadius
                y: 0
                radiusX: PM.ControlRadius
                radiusY: PM.ControlRadius
            }
        }
    }

    // 尖角向左
    Shape {
        visible: SliderHandle.ArrowPosition.ArrowLeft === control.arrowPosition ? true : false
        ShapePath {
            strokeColor: control.color
            fillColor: control.color
            fillRule: ShapePath.WindingFill
            startX: PM.ControlRadius
            startY: 0
            PathLine {
                x: control.width - PM.ControlRadius
                y: 0
            }
            PathArc {
                x: control.width
                y: PM.ControlRadius
                radiusX: PM.ControlRadius
                radiusY: PM.ControlRadius
            }
            PathLine {
                x: control.width
                y: control.height - PM.ControlRadius
            }
            PathArc {
                x: control.width - PM.ControlRadius
                y: control.height
                radiusX: PM.ControlRadius
                radiusY: PM.ControlRadius
            }
            PathLine {
                x: PM.ControlRadius
                y: control.height
            }
            PathLine {
                x: 0
                y: control.height / 2
            }
            PathLine {
                x: PM.ControlRadius
                y: 0
            }
        }
    }

    // 尖角向右
    Shape {
        visible: SliderHandle.ArrowPosition.ArrowRight === control.arrowPosition ? true : false
        ShapePath {
            strokeColor: control.color
            fillColor: control.color
            fillRule: ShapePath.WindingFill
            startX: PM.ControlRadius
            startY: 0
            PathLine {
                x: control.width - PM.ControlRadius
                y: 0
            }
            PathLine {
                x: control.width
                y: control.height / 2
            }
            PathLine {
                x: control.width - PM.ControlRadius
                y: control.height
            }
            PathLine {
                x: PM.ControlRadius
                y: control.height
            }
            PathArc {
                x: 0
                y: control.height - PM.ControlRadius
                radiusX: PM.ControlRadius
                radiusY: PM.ControlRadius
            }
            PathLine {
                x: 0
                y: PM.ControlRadius
            }
            PathArc {
                x: PM.ControlRadius
                y: 0
                radiusX: PM.ControlRadius
                radiusY: PM.ControlRadius
            }
        }
    }
}
