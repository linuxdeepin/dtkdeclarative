/*
 * Copyright (C) 2022 UnionTech Technology Co., Ltd.
 *
 * Author:     Chen Bin <chenbin@uniontech.com>
 *
 * Maintainer: Chen Bin <chenbin@uniontech.com>
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
import org.deepin.dtk 1.0 as D
import org.deepin.dtk.style 1.0 as DS

Control {
    id: control
    property int value /*0~100*/

    background: BoxShadow {
        implicitWidth: DS.Style.progressBar.water.width
        implicitHeight: DS.Style.progressBar.water.height
        shadowColor: DS.Style.progressBar.water.shadowColor
        shadowOffsetY: 6
        shadowBlur: 6
        cornerRadius: width / 2

        Rectangle {
            anchors.fill: parent
            radius: parent.cornerRadius
            gradient: Gradient {
                GradientStop { position: 0.0; color: DS.Style.progressBar.water.backgroundColor1 }
                GradientStop { position: 1; color: DS.Style.progressBar.water.backgroundColor2 }
            }
        }
    }

    contentItem: Item {
        Item {
            id: content
            anchors.fill: parent
            D.WaterProgressAttribute {
                id: attribute
                running: control.value > 0
                waterProgress: control
                imageWidth: 5 * control.width
                imageHeight: 1.1 * control.height
            }

            Repeater {
                model: 4
                Image {
                    readonly property real xoffset: index < 2 ? attribute.backXOffset : attribute.frontXOffset
                    source: index < 2 ? DS.Style.progressBar.water.waterBackImagePath
                                      : DS.Style.progressBar.water.waterFrontImagePath
                    x: index % 2 ? xoffset - width : xoffset
                    y: (90 - control.value) * control.height / 100
                    width: attribute.imageWidth
                    height: attribute.imageHeight
                }
            }

            Repeater {
                model: attribute.pops

                Rectangle {
                    color: DS.Style.progressBar.water.popBackgroundColor
                    visible: control.value > 30
                    x: model.x
                    y: model.y
                    width: model.width
                    height: model.height
                    radius: width / 2
                }
            }

            Text {
                anchors.centerIn: parent
                text: control.value
                horizontalAlignment: Qt.AlignHCenter
                verticalAlignment: Qt.AlignVCenter
                font {
                    pixelSize: control.height * 0.4
                }
                color: DS.Style.progressBar.water.textColor
            }
        }

        D.ItemViewport {
            anchors.fill: parent
            sourceItem: content
            hideSource: true
            radius: width / 2
        }
    }
}
