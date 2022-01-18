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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

import QtQuick 2.0
import QtQuick 2.6
import QtQuick.Window 2.11
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.11
import org.deepin.dtk 1.0 as D
import org.deepin.dtk.style 1.0 as DS

Column {
    id: control

    property string longMessage: "Copyright 2014-2019 Adobe (http://www.adobe.com/), with Reserved FontName 'Source'. Source is a trademark of Adobe in the United States and/or other countries."
    property string shortMessage: "short message"

    Button {
        text: "FloatingMessage"
        property int count: 0
        onClicked: {
            if (count % 2) {
                D.DTK.sendMessage(control, "message" + count)
                D.DTK.sendMessage(Window.window, "message" + count, "music", 4000, "type1")
                D.DTK.sendMessage(Window.window, "message" + count, "video", -1)
            } else {
                D.DTK.sendMessage(Window.window, floatingMsgCom, {content: shortMessage}, -1)
                D.DTK.sendMessage(Window.window, floatingMsgCom, {content: longMessage, iconName: "music"}, -1)
            }
            count++
        }
    }

    Component {
        id: floatingMsgCom
        D.FloatingMessage {
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
                    Layout.alignment: Qt.AlignVCenter
                    font: D.DTK.fontManager.t6
                    wrapMode: Text.Wrap
                    elide: Text.ElideRight
                    maximumLineCount: 1
                }

                Button {
                    text: "reload"
                    font: D.DTK.fontManager.t5
                    Layout.alignment: Qt.AlignVCenter
                }
            }
//            button: Button {
//                text: "close"
//                onClicked: {
//                    console.log("close clicked")
//                    D.DTK.closeMessage(floatMsg)
//                }
//            }
        }
    }
}
