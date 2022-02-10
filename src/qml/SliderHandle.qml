/*
 * Copyright (C) 2022 UnionTech Technology Co., Ltd.
 *
 * Author:     xiaoyaobing <xiaoyaobing@uniontech.com>
 *
 * Maintainer: xiaoyaobing <xiaoyaobing@uniontech.com>
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
import QtQuick.Controls 2.4
import QtQuick.Shapes 1.11
import org.deepin.dtk 1.0 as D

// TODO(Xiao Yao Bing): Replace it with dci icon.
Item {
    id: control

    property color color: "red"
    property int arrowPosition: 0
    property real radius

    enum ArrowPosition {
        NoArrow = 0,
        ArrowAbove = 1,
        ArrowLeft = 2,
        ArrowBelow = 3,
        ArrowRight = 4
    }

    layer.enabled: true
    layer.samples: 4
    smooth: true

    // NoArrow
    Rectangle {
        visible: SliderHandle.ArrowPosition.NoArrow === control.arrowPosition ? true : false
        x: 0
        y: 0
        width: control.width
        height: control.height
        radius: control.radius
        color: control.color
    }

    // ArrowAbove
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
                y: control.radius
            }
            PathLine {
                x: control.width
                y: control.height - control.radius
            }
            PathArc {
                x: control.width - control.radius
                y: control.height
                radiusX: control.radius
                radiusY: control.radius
            }
            PathLine {
                x: control.radius
                y: control.height
            }
            PathArc {
                x: 0
                y: control.height - control.radius
                radiusX: control.radius
                radiusY: control.radius
            }
            PathLine {
                x: 0
                y: control.radius
            }
            PathLine {
                x: control.width / 2
                y: 0
            }
        }
    }

    // ArrowBelow
    Shape {
        visible: SliderHandle.ArrowPosition.ArrowBelow === control.arrowPosition ? true : false
        ShapePath {
            strokeColor: control.color
            fillColor: control.color
            fillRule: ShapePath.WindingFill
            startX: control.radius
            startY: 0
            PathLine {
                x: control.width - control.radius
                y: 0
            }
            PathArc {
                x: control.width
                y: control.radius
                radiusX: control.radius
                radiusY: control.radius
            }
            PathLine {
                x: control.width
                y: control.height - control.radius
            }
            PathLine {
                x: control.width / 2
                y: control.height
            }
            PathLine {
                x: 0
                y: control.height - control.radius
            }
            PathLine {
                x: 0
                y: control.radius
            }
            PathArc {
                x: control.radius
                y: 0
                radiusX: control.radius
                radiusY: control.radius
            }
        }
    }

    // ArrowLeft
    Shape {
        visible: SliderHandle.ArrowPosition.ArrowLeft === control.arrowPosition ? true : false
        ShapePath {
            strokeColor: control.color
            fillColor: control.color
            fillRule: ShapePath.WindingFill
            startX: control.radius
            startY: 0
            PathLine {
                x: control.width - control.radius
                y: 0
            }
            PathArc {
                x: control.width
                y: control.radius
                radiusX: control.radius
                radiusY: control.radius
            }
            PathLine {
                x: control.width
                y: control.height - control.radius
            }
            PathArc {
                x: control.width - control.radius
                y: control.height
                radiusX: control.radius
                radiusY: control.radius
            }
            PathLine {
                x: control.radius
                y: control.height
            }
            PathLine {
                x: 0
                y: control.height / 2
            }
            PathLine {
                x: control.radius
                y: 0
            }
        }
    }

    // ArrowRight
    Shape {
        visible: SliderHandle.ArrowPosition.ArrowRight === control.arrowPosition ? true : false
        ShapePath {
            strokeColor: control.color
            fillColor: control.color
            fillRule: ShapePath.WindingFill
            startX: control.radius
            startY: 0
            PathLine {
                x: control.width - control.radius
                y: 0
            }
            PathLine {
                x: control.width
                y: control.height / 2
            }
            PathLine {
                x: control.width - control.radius
                y: control.height
            }
            PathLine {
                x: control.radius
                y: control.height
            }
            PathArc {
                x: 0
                y: control.height - control.radius
                radiusX: control.radius
                radiusY: control.radius
            }
            PathLine {
                x: 0
                y: control.radius
            }
            PathArc {
                x: control.radius
                y: 0
                radiusX: control.radius
                radiusY: control.radius
            }
        }
    }
}
