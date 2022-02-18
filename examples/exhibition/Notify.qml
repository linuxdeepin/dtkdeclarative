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
    }
}
