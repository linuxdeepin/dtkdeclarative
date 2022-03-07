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

import QtQuick 2.0
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.11
import QtQuick.Window 2.11
import org.deepin.dtk 1.0

Column {
    id: control
    Label {
        width: parent.width
        wrapMode: Text.WordWrap
        text: "应用内通知。"
        horizontalAlignment: Qt.AlignHCenter
    }

    spacing: 10

    Flow {
        spacing: 10
        width: parent.width

        Button {
            text: "send buildin type message"
            onClicked: {
                DTK.sendMessage(control, "成功添加到＂校园名谣＂", "checked")
            }
        }

        Button {
            text: "send custom type message"
            onClicked: DTK.sendMessage(Window.window, floatingMsgCom, {
                                           content: "磁盘中的原文件已被修改，是否重新载入？",
                                           iconName: "music"
                                       }, -1)
        }

        Component {
            id: floatingMsgCom
            FloatingMessage {
                id: floatMsg
                contentItem: RowLayout {
                    anchors.fill: parent
                    spacing: 0

                    Label {
                        Layout.fillHeight: true
                        Layout.fillWidth: true
                        text: floatMsg.message.content
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignLeft
                        elide: Text.ElideRight
                        maximumLineCount: 1
                        font: DTK.fontManager.t6
                    }

                    Button {
                        text: "reload"
                        font: DTK.fontManager.t5
                        Layout.alignment: Qt.AlignVCenter
                        Layout.preferredWidth: 72
                        Layout.preferredHeight: 28
                    }
                }
            }
        }

        Item {width: parent.width; height: 1}

        Row {
            spacing: 150
            Loader {
                sourceComponent: appBadgeCom
                property int number: 1
                property string appName: "deepin-app-store"
            }
            Loader {
                sourceComponent: appBadgeCom
                property int number: 99
                property string appName: "deepin-browser"
            }
            Loader {
                sourceComponent: appBadgeCom
                property int number: 100
                property string appName: "deepin-editor"
            }
            Loader {
                sourceComponent: appBadgeCom
                property int number: 1000
                property string appName: "deepin-diskmanager"
            }
            Component {
                id: appBadgeCom

                QtIcon {
                    name: appName
                    sourceSize {
                        width: 128
                        height: width
                    }
                    Label {
                        height: 20
                        width: Math.max(20, implicitWidth)
                        z: DTK.AboveOrder
                        anchors {
                            right: parent.right
                            rightMargin: 4
                            top: parent.top
                            topMargin: 4
                        }
                        topPadding: 5
                        rightPadding: 7
                        leftPadding: 7
                        text: {
                            if (number >= 0 && number < 100) {
                                return number
                            } else if (number >= 100 && number < 1000) {
                                return number + "+"
                            }
                            return "!"
                        }
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        font: DTK.fontManager.t5
                        color: palette.window
                        background: Rectangle {
                            implicitHeight: 20
                            implicitWidth: 20
                            radius: 10
                            BoxShadow {
                                anchors.fill: parent
                                shadowBlur: 3
                                shadowOffsetY: 2
                                shadowColor: Qt.rgba(255, 113, 113, 0.3)
                                cornerRadius: 0
                            }
                            gradient: Gradient {
                                GradientStop { position: 0.0; color: "#ff674a" }
                                GradientStop { position: 1.0; color: "#ec5783" }
                            }
                        }
                    }
                }
            }
        }

        Item {width: parent.width; height: 1}
    }
}
