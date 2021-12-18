/*
 * Copyright (C) 2021 Uniontech Technology Co., Ltd.
 *
 * Author:     yeshanshan <yeshanshan@uniontech.com>
 *
 * Maintainer: yeshanshan <yeshanshan@uniontech.com>
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
import QtGraphicalEffects 1.0
import org.deepin.dtk.impl 1.0 as D

Item{
    id: control
    height: source.height
    width: source.width
    layer.enabled: true
    layer.effect: OpacityMask {
        source: control
        maskSource: Rectangle {
            width: control.source.width
            height: control.source.height
            radius: control.source.radius || 0
        }
    }

    // hover virtual item.
    property var source: null
    // the area's base color
    property color hoverColor: Qt.red
    // to select center point
    property point centerPoint: Qt.point(0, 0)

    // to start animation
    function start() {
        // it's not be binding due to a pair for increasing and decreasing.
        impl.color = (D.DTK.toColorType(hoverColor) === D.ApplicationHelper.LightType) ? "black": "white"

        impl.__increase()
    }

    // to stop animation
    function stop() {
        impl.__scalePos = 0
    }

    Rectangle {
        id: impl
        height: width
        opacity: 0.5
        radius: width / 2
        scale: __scalePos / 50

        property real __scalePos: 0
        Behavior on __scalePos {
            SmoothedAnimation {
                id: smoothedAnimation
                velocity: 200
            }
        }

        function __increase() {
            var curRadius = __minBoundingCicleRadius()

            impl.width = curRadius * 2
            impl.x = centerPoint.x - curRadius
            impl.y = centerPoint.y - curRadius
            impl.__scalePos = 50
        }
        function __minBoundingCicleRadius()
        {
            // 区域内任意一点到四个定点距离的最大值，即为以此点为中心包含此区域的最小圆的半径
            var r1 = Math.sqrt(Math.pow(centerPoint.x, 2) + Math.pow(centerPoint.y, 2))
            var r2 = Math.sqrt(Math.pow(centerPoint.x, 2) + Math.pow(
                                   control.height - centerPoint.y, 2))
            var r3 = Math.sqrt(Math.pow(centerPoint.y, 2) + Math.pow(
                                   control.width - centerPoint.x, 2))
            var r4 = Math.sqrt(Math.pow(control.width - centerPoint.x,
                                        2) + Math.pow(
                                   control.height - centerPoint.y, 2))
            return Math.max(r1, r2, r3, r4)
        }
    }
}

