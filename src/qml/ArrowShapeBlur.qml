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

import QtQuick 2.0
import QtQuick.Shapes 1.11
import QtGraphicalEffects 1.0
import org.deepin.dtk.impl 1.0 as D
import org.deepin.dtk.style 1.0 as DS
import "private"

D.InWindowBlur {
    id: __item

    property color blendColor
    property color borderColor
    property real borderWidth: DS.Style.arrowRectangleBlur.borderWidth
    property real roundJoinRadius: DS.Style.control.radius
    property real arrowWidth: __arrowVertical() ? __item.width : __item.height
    property real arrowHeight: __item.height / DS.Style.arrowRectangleBlur.arrowHeightFactor
    property point arrowPosition: Qt.point(__getArrowX(), __getArrowY())
    property int arrowDirection: D.DTK.DownArrow
    property alias shadowColor: __shadow.color

    radius: DS.Style.arrowRectangleBlur.blurRadius
    offscreen: true

    function __arrowVertical() {
        return D.DTK.UpArrow === __item.arrowDirection
                || D.DTK.DownArrow === __item.arrowDirection
    }

    function __getArrowX() {
        if (__arrowVertical())
            return __item.width / 2
        else if (D.DTK.LeftArrow === __item.arrowDirection)
            return 0
        else if (D.DTK.RightArrow === __item.arrowDirection)
            return __item.width;
        else
            return __item.width / 2
    }

    function __getArrowY() {
        if (!__arrowVertical())
            return __item.height / 2
        else if (D.DTK.UpArrow === __item.arrowDirection)
            return 0
        else
            return __item.height
    }

    DropShadow {
        id: __shadow
        anchors.fill: __loader
        verticalOffset: 6
        radius: 16.0
        samples: 17
        source: __loader
    }

    D.ItemViewport {
        id: _viewPort
        anchors.fill: parent
        fixed: true
        sourceItem: parent
        hideSource: false
        visible: false

        Rectangle {
            anchors.fill: parent
            color: __item.blendColor
        }
    }

    Loader {
        id: __loader
        visible: false
        anchors.fill: parent
        sourceComponent: {
            switch(__item.arrowDirection) {
            case D.DTK.UpArrow:
                return upArrow
            case D.DTK.LeftArrow:
                return leftArrow
            case D.DTK.RightArrow:
                return rightArrow
            case D.DTK.DownArrow:
                return downArrow
            default:
                console.error("Unknown arrowDirection enumeration type!")
            }
        }
    }

    OpacityMask {
        id: __mask
        anchors.fill: _viewPort
        source: _viewPort
        maskSource: __loader
    }

    // border
    Loader {
        anchors.fill: parent
        sourceComponent: {
            if (!__item.borderWidth > 0 || !__item.borderColor.a > 0)
                return undefined

            switch(__item.arrowDirection) {
            case D.DTK.UpArrow:
                return upArrow
            case D.DTK.LeftArrow:
                return leftArrow
            case D.DTK.RightArrow:
                return rightArrow
            case D.DTK.DownArrow:
                return downArrow
            default:
                console.error("Unknown arrowDirection enumeration type!")
            }
        }

        onLoaded: {
            item.strokeWidth = __item.borderWidth
            item.strokeColor = __item.borderColor
            item.fillColor = "transparent"
        }
    }

    Component {
        id: downArrow
        Shape {
            property alias strokeWidth: __path.strokeWidth
            property alias strokeColor: __path.strokeColor
            property alias fillColor: __path.fillColor
            DownArrowShapePath {
                id: __path
                width: __item.width
                height: __item.height
                roundJoinRadius: __item.roundJoinRadius
                arrowPosition: __item.arrowPosition
                arrowWidth: __item.arrowWidth
                arrowHeight: __item.arrowHeight
                arrowDirection: __item.arrowDirection
                strokeWidth: 0
                strokeColor: "transparent"
            }
        }
    }

    Component {
        id: upArrow
        Shape {
            property alias strokeWidth: __path.strokeWidth
            property alias strokeColor: __path.strokeColor
            property alias fillColor: __path.fillColor
            UpArrowShapePath {
                id: __path
                width: __item.width
                height: __item.height
                roundJoinRadius: __item.roundJoinRadius
                arrowPosition: __item.arrowPosition
                arrowWidth: __item.arrowWidth
                arrowHeight: __item.arrowHeight
                arrowDirection: __item.arrowDirection
                strokeWidth: 0
                strokeColor: "transparent"
            }
        }
    }

    Component {
        id: leftArrow
        Shape {
            property alias strokeWidth: __path.strokeWidth
            property alias strokeColor: __path.strokeColor
            property alias fillColor: __path.fillColor
            LeftArrowShapePath {
                id: __path
                width: __item.width
                height: __item.height
                roundJoinRadius: __item.roundJoinRadius
                arrowPosition: __item.arrowPosition
                arrowWidth: __item.arrowWidth
                arrowHeight: __item.arrowHeight
                arrowDirection: __item.arrowDirection
                strokeWidth: 0
                strokeColor: "transparent"
            }
        }
    }

    Component {
        id: rightArrow
        Shape {
            property alias strokeWidth: __path.strokeWidth
            property alias strokeColor: __path.strokeColor
            property alias fillColor: __path.fillColor
            RightArrowShapePath {
                id: __path
                width: __item.width
                height: __item.height
                roundJoinRadius: __item.roundJoinRadius
                arrowPosition: __item.arrowPosition
                arrowWidth: __item.arrowWidth
                arrowHeight: __item.arrowHeight
                arrowDirection: __item.arrowDirection
                strokeWidth: 0
                strokeColor: "transparent"
            }
        }
    }
}
