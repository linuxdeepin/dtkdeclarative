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

import QtQuick.Shapes 1.11
import org.deepin.dtk.impl 1.0 as D
import org.deepin.dtk.style 1.0 as DS

ShapePath {
    property real roundJoinRadius
    property real width
    property real height
    property point arrowPosition
    property real arrowWidth
    property real arrowHeight
    property int arrowDirection

    readonly property real arrowLeftRaidus: getArrowLeftRaidus()
    readonly property real arrowRightRaidus: getArrowRightRaidus()

    startX: arrowPosition.x
    startY: arrowPosition.y
    fillRule: ShapePath.WindingFill

    function getArrowLeftRaidus() {
        switch(arrowDirection) {
        case D.DTK.UpArrow:
            return arrowPosition.x - arrowWidth / 2 > 0 ? roundJoinRadius : 0
        case D.DTK.LeftArrow:
            return arrowPosition.y + arrowWidth / 2 < height ? roundJoinRadius : 0
        case D.DTK.RightArrow:
            return arrowPosition.y - arrowWidth / 2 > 0 ? roundJoinRadius : 0
        case D.DTK.DownArrow:
            return arrowPosition.x + arrowWidth / 2 < width ? roundJoinRadius : 0
        default:
            console.error("Unknown arrowDirection enumeration type!")
            return 0
        }
    }

    function getArrowRightRaidus() {
        switch(arrowDirection) {
        case D.DTK.UpArrow:
            return arrowPosition.x + arrowWidth / 2 < width ? roundJoinRadius : 0
        case D.DTK.LeftArrow:
            return arrowPosition.y - arrowWidth / 2 > 0 ? roundJoinRadius : 0
        case D.DTK.RightArrow:
            return arrowPosition.y + arrowWidth / 2 < height ? roundJoinRadius : 0
        case D.DTK.DownArrow:
            return arrowPosition.x - arrowWidth / 2 > 0 ? roundJoinRadius : 0
        default:
            console.error("Unknown arrowDirection enumeration type!")
            return 0
        }
    }
}
