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
import QtQml 2.11
import QtQuick.Layouts 1.11
import org.deepin.dtk 1.0

Column {
    id: panel
    Label {
        width: parent.width
        wrapMode: Text.WordWrap
        text: "所有需要用户等待的地方,且没有具体的等待时间,不知道进度,可能很快也可能需要比较久。"
        horizontalAlignment: Qt.AlignHCenter
    }

    spacing: 10

    Flow {
        spacing: 10
        width: parent.width

        RowLayout {
            spacing: 50
            Repeater {
                model: [16, 32, 48, 64]
                delegate: BusyIndicator {
                    running: true
                    Layout.preferredWidth: modelData
                    Layout.preferredHeight: modelData
                }
            }
        }

        Item { width: 100; height: 1}

        RowLayout {
            spacing: 50
            Repeater {
                model: [16, 32, 48, 64]
                delegate: Rectangle {
                    Layout.preferredWidth: modelData
                    Layout.preferredHeight: modelData
                    color: "#8a8a8a"
                    BusyIndicator {
                        anchors.fill: parent
                        running: true
                        fillColor: Palette {
                            normal: "#ffffff"
                        }
                    }
                }
            }
        }
    }

}
