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

import QtQuick 2.4
import QtQuick.Window 2.11
import org.deepin.dtk.impl 1.0 as D
import org.deepin.dtk.style 1.0 as DS
import "private"

Window {
    id: __window

    property real roundJoinRadius: DS.Style.control.radius
    property real arrowWidth: __arrowVertical() ? __window.width : __window.height
    property real arrowHeight: __window.height / DS.Style.arrowRectangleBlur.arrowHeightFactor
    property point arrowPosition: Qt.point(__getArrowX(), __getArrowY())
    property int arrowDirection: D.DTK.DownArrow

    D.DWindow.enabled: true
    D.DWindow.clipPath: loader.item
    D.DWindow.enableBlurWindow: true
    D.DWindow.alphaBufferSize: D.DTK.hasBlurWindow ? 8 : 0

    function __arrowVertical() {
        return D.DTK.UpArrow === __window.arrowDirection
                || D.DTK.DownArrow === __window.arrowDirection
    }

    function __getArrowX() {
        if (__arrowVertical())
            return __window.width / 2
        else if (D.DTK.LeftArrow === __window.arrowDirection)
            return 0
        else if (D.DTK.RightArrow === __window.arrowDirection)
            return __window.width;
        else
            return __window.width / 2
    }

    function __getArrowY() {
        if (!__arrowVertical())
            return __window.height / 2
        else if (D.DTK.UpArrow === __window.arrowDirection)
            return 0
        else
            return __window.height
    }

    Loader {
        id: loader
        anchors.fill: parent
        sourceComponent: {
            switch(__window.arrowDirection) {
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

    Component {
        id: downArrow
        DownArrowShapePath {
            width: __window.width
            height: __window.height
            roundJoinRadius: __window.roundJoinRadius
            arrowPosition: __window.arrowPosition
            arrowWidth: __window.arrowWidth
            arrowHeight: __window.arrowHeight
            arrowDirection: __window.arrowDirection
        }
    }

    Component {
        id: upArrow
        UpArrowShapePath {
            width: __window.width
            height: __window.height
            roundJoinRadius: __window.roundJoinRadius
            arrowPosition: __window.arrowPosition
            arrowWidth: __window.arrowWidth
            arrowHeight: __window.arrowHeight
            arrowDirection: __window.arrowDirection
        }
    }

    Component {
        id: leftArrow
        LeftArrowShapePath {
            width: __window.width
            height: __window.height
            roundJoinRadius: __window.roundJoinRadius
            arrowPosition: __window.arrowPosition
            arrowWidth: __window.arrowWidth
            arrowHeight: __window.arrowHeight
            arrowDirection: __window.arrowDirection
        }
    }

    Component {
        id: rightArrow
        RightArrowShapePath {
            width: __window.width
            height: __window.height
            roundJoinRadius: __window.roundJoinRadius
            arrowPosition: __window.arrowPosition
            arrowWidth: __window.arrowWidth
            arrowHeight: __window.arrowHeight
            arrowDirection: __window.arrowDirection
        }
    }
}
