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
import org.deepin.dtk.impl 1.0 as D

Item {
    id: root

    // to select center point
    property point centerPoint: Qt.point(0, 0)

    layer {
        enabled: false
        effect: D.OpacityMask {
            maskSource: mask
        }
    }

    // to start animation
    function start() {
        if (D.DTK.isSoftwareRender)
            return

        mask.active = true
        layer.enabled = true
        mask.item.__increase()
    }

    // to stop animation
    function stop() {
        layer.enabled = false
        mask.active = false
        mask.__scalePos = 0
    }

    Loader {
        id: mask
        anchors.fill: parent
        visible: false

        property real __scalePos: 0
        Behavior on __scalePos {
            SmoothedAnimation { velocity: 300 }
        }

        sourceComponent: Rectangle {
            id: impl
            height: width
            radius: width / 2
            scale: __scalePos / 50

            function __increase() {
                var curRadius = __minBoundingCicleRadius()

                impl.width = curRadius * 2
                impl.x = centerPoint.x - curRadius
                impl.y = centerPoint.y - curRadius
                mask.__scalePos = 50
            }
            function __minBoundingCicleRadius() {
                // 区域内任意一点到四个定点距离的最大值，即为以此点为中心包含此区域的最小圆的半径
                var r1 = Math.sqrt(Math.pow(centerPoint.x, 2) + Math.pow(centerPoint.y, 2))
                var r2 = Math.sqrt(Math.pow(centerPoint.x, 2) + Math.pow(
                                       root.height - centerPoint.y, 2))
                var r3 = Math.sqrt(Math.pow(centerPoint.y, 2) + Math.pow(
                                       root.width - centerPoint.x, 2))
                var r4 = Math.sqrt(Math.pow(root.width - centerPoint.x,
                                            2) + Math.pow(
                                       root.height - centerPoint.y, 2))
                return Math.max(r1, r2, r3, r4)
            }
        }
    }
}

