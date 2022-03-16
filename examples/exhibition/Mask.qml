/*
 * Copyright (C) 2022 UnionTech Technology Co., Ltd.
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
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

import QtQuick 2.11
import QtQuick.Layouts 1.11
import QtQuick.Window 2.0
import org.deepin.dtk 1.0

Column {
    id: control
    Label {
        width: parent.width
        wrapMode: Text.WordWrap
        text: "用于展示异形窗口和异形控件的实现。"
        horizontalAlignment: Qt.AlignHCenter
    }

    spacing: 10

    Flow {
        spacing: 30
        width: parent.width

//        StyledArrowShapeBlur {
//            width: 100
//            height: 200
//            Label {
//                anchors.centerIn: parent
//                text: "箭头模糊控件"
//            }
//        }

//        Button {
//            text: "箭头异形窗口"
//            onClicked: shapeWindow1.show()
//            StyledArrowShapeWindow {
//                id: shapeWindow1
//                width: 100
//                height: 200
//                Label {
//                    anchors.centerIn: parent
//                    text: "箭头异形窗口"
//                }
//            }
//        }

        Button {
            text: "自定义异形窗口"
            onClicked: shapeWindow2.show()
            Window {
                id: shapeWindow2
                width: 200
                height: 200
                DWindow.enabled: true
                DWindow.enableBlurWindow: true
                DWindow.clipPath: Path {
                    id: p2
                    property int width: shapeWindow2.width
                    property int height: shapeWindow2.height
                    property real r: 50
                    startX: 0
                    startY: p2.height / 2 - r
                    PathArc {
                        x: p2.width / 2 + p2.r
                        y: p2.height / 2 + p2.r
                        radiusX: p2.r + 10; radiusY: p2.r
                        useLargeArc: true
                    }
                    PathArc {
                        x: p2.width / 2 - p2.r
                        y: p2.height / 2 - p2.r
                        radiusX: p2.r + 10; radiusY: p2.r
                        useLargeArc: true
                    }
                }
                Label {
                    anchors.centerIn: parent
                    text: "自定义异形窗口"
                }
            }
        }
    }
}
