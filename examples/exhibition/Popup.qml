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
import QtQuick.Layouts 1.11
import QtQuick.Window 2.11
import QtQml.Models 2.11
import org.deepin.dtk 1.0
import ".."

Column {
    id: control
    Label {
        width: parent.width
        wrapMode: Text.WordWrap
        text: "弹窗类控件，包括弹出菜单、弹出窗口、对话框等。"
        horizontalAlignment: Qt.AlignHCenter
    }

    spacing: 10

    Flow {
        spacing: 10
        width: parent.width

        ImageCell {
            source: "qrc:/assets/popup/popup-menu-1.svg"
            Popup {
                width: 200; height: 280
                ColumnLayout {
                    width: parent.width
                    Label {
                        text: "勿扰模式"
                        Layout.alignment: Qt.AlignHCenter
                        font: DTK.fontManager.t5
                    }
                    Switch {
                        checked: true
                        Layout.alignment: Qt.AlignHCenter
                    }
                    Label {
                        text: {
                            if (popupListView.currentItem.currentIndex < 0)
                                return ""
                            var item = popupListView.model.get(popupListView.currentItem.currentIndex)
                            switch (item.type) {
                            case 1:
                                return "今晚7: 00前保持开启"
                            }
                            return ""
                        }
                        Layout.alignment: Qt.AlignHCenter
                        font: DTK.fontManager.t7
                    }
                    ListView {
                        id: popupListView
                        Layout.preferredHeight: contentHeight
                        spacing: 10
                        model: ListModel {
                            ListElement { type: 1; icon: "music"; text: "1小时"}
                            ListElement { type: 2; icon: "music"; text: "直至今晚"}
                            ListElement { type: 3; icon: "music"; text: "直至明早"}
                            ListElement { type: 4; icon: "music"; text: "保持开启"}
                        }
                        delegate: ItemDelegate {
                            width: 180
                            icon.name: model.icon
                            text: model.text
                            checked: index === 1
                        }
                    }
                }
            }
        }

        ImageCell {
            source: "qrc:/assets/popup/popup-menu-2.svg"
            Popup {
                width: 200; height: 370
                ColumnLayout {
                    width: parent.width
                    Label {
                        text: "蓝牙"
                        Layout.alignment: Qt.AlignHCenter
                        font: DTK.fontManager.t5
                    }
                    Switch {
                        checked: true
                        Layout.alignment: Qt.AlignHCenter
                    }

                    MenuSeparator { text: "我的设备"}
                    ListView {
                        Layout.preferredHeight: contentHeight
                        Layout.preferredWidth: 180
                        Layout.alignment: Qt.AlignHCenter
                        spacing: 10
                        model: ObjectModel {
                            ItemDelegate {width: 180; text: "WH-1000MX3"; icon.name: "music"; checked: false }
                            ItemDelegate {width: 180; text: "我的音响"; icon.name: "music"; checked: true }
                            ItemDelegate {width: 180; text: "蓝牙鼠标"; icon.name: "music"; checked: false }
                        }
                    }

                    MenuSeparator { text: "其它设备"}
                    ListView {
                        Layout.preferredHeight: contentHeight
                        Layout.preferredWidth: 180
                        Layout.alignment: Qt.AlignHCenter
                        spacing: 10
                        model: ObjectModel {
                            ItemDelegate {width: 180; text: "SHULE's PC"; icon.name: "music" }
                            ItemDelegate {width: 180; text: "打印机"; icon.name: "music" }
                        }
                    }
                }
            }
        }

        ImageCell {
            source: "qrc:/assets/popup/popup-menu-3.svg"
            Popup {
                width: 280; height: 250
                ColumnLayout {
                    width: parent.width
                    Label {
                        text: "声音"
                        Layout.alignment: Qt.AlignHCenter
                        font: DTK.fontManager.t5
                    }
                    RowLayout {
                        QtIcon { name: "irc-voice" }

                        Slider {
                            Layout.alignment: Qt.AlignHCenter
                            highlightedPassedGroove: true
                            Layout.fillWidth: true
                            from: 0
                            to: 100
                            value: 50
                        }

                        QtIcon { name: "irc-voice" }
                    }

                    MenuSeparator { text: "输出"}
                    ListView {
                        Layout.preferredHeight: contentHeight
                        Layout.preferredWidth: 250
                        Layout.alignment: Qt.AlignHCenter
                        spacing: 10
                        model: ObjectModel {
                            ItemDelegate {width: 250; text: "扬声器"; icon.name: "music"; checked: true }
                            ItemDelegate {width: 250; text: "WH-1000MX3"; icon.name: "music"; checked: false }
                            ItemDelegate {width: 250; text: "声音设置"; icon.name: "music"; checked: false }
                        }
                    }
                }
            }
        }

        ImageCell {
            source: "qrc:/assets/popup/popup-menu-4.svg"
            Popup {
                width: 250; height: 530
                ColumnLayout {
                    width: parent.width
                    Label {
                        text: "蓝牙"
                        Layout.alignment: Qt.AlignHCenter
                        font: DTK.fontManager.t5
                    }
                    Switch {
                        checked: true
                        Layout.alignment: Qt.AlignHCenter
                    }

                    ButtonGroup {
                        id: networkGroup
                    }
                    ListModel {
                        id: networkModel
                        ListElement { type: 1; signalLevel: 1; text: "Uniontech"}
                        ListElement { type: 1; signalLevel: 2; text: "TP-link_1723"}
                        ListElement { type: 2; signalLevel: 1; text: "舒乐的热点"}
                        ListElement { type: 2; signalLevel: 1; text: "花花小仙女WIFI"}
                        ListElement { type: 2; signalLevel: 1; text: "有本事你来蹭啊"}
                        ListElement { type: 2; signalLevel: 2; text: "Uniontech-M"}
                        ListElement { type: 2; signalLevel: 2; text: "Miwifi"}
                        ListElement { type: 2; signalLevel: 2; text: "Guest"}
                        ListElement { type: 2; signalLevel: 2; text: "网络设置"}
                    }

                    MenuSeparator { text: "我的网络"}
                    ListView {
                        Layout.preferredHeight: contentHeight
                        Layout.preferredWidth: 220
                        Layout.alignment: Qt.AlignHCenter
                        spacing: 10
                        model: SortFilterModel {
                            model: networkModel
                            delegate: ItemDelegate {
                                width: 220
                                icon.name: {
                                    if (model.signalLevel === 1)
                                        return "music"
                                    return "irc-voice"
                                }
                                text: model.text
                                checked: index === 1
                                ButtonGroup.group: networkGroup
                            }
                            filterAcceptsItem: function(item) {
                                return item.type === 1
                            }
                            lessThan: null
                        }
                    }

                    ItemDelegate {
                        Layout.fillWidth: true; Layout.preferredHeight: 24;
                        topPadding: 0
                        bottomPadding: 0
                        text: "其它网络";
                        icon.name: networkView.visible ? "go-up": "go-down"
                        display: IconLabel.IconBesideText
                        checkable: false
                        font: DTK.fontManager.t8
                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                networkView.visible = !networkView.visible
                            }
                        }
                    }

                    ListView {
                        id: networkView
                        Layout.preferredHeight: contentHeight
                        Layout.preferredWidth: 220
                        Layout.alignment: Qt.AlignHCenter
                        spacing: 2
                        model: SortFilterModel {
                            model: networkModel

                            delegate: ItemDelegate {
                                width: 220
                                icon.name: {
                                    if (model.signalLevel === 1)
                                        return "music"
                                    return "irc-voice"
                                }
                                text: model.text
                                corners: getCornersForBackground(DelegateModel.visibleIndex, ListView.view.count)
                                ButtonGroup.group: networkGroup
                            }
                            filterAcceptsItem: function(item) {
                                return item.type === 2
                            }
                            lessThan: null
                        }
                    }
                }
            }
        }
    }
}
