/*
 * Copyright (C) 2022 UnionTech Technology Co., Ltd.
 *
 * Author:     JiDe Zhang <zhangjide@deepin.org>
 *
 * Maintainer: JiDe Zhang <zhangjide@deepin.org>
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
import QtQuick.Controls 2.4
// 确保在最后引入
import org.deepin.dtk 1.0

ApplicationWindow {
    id: root
    visible: true
    width: Math.max(contentList.contentWidth, 800)
    height: 600
    title: qsTr("DTK Exhibition")

    // 开启“圆角窗口&无系统标题栏”模式
    Window.enabled: true

    ButtonGroup {
        id: activeColorSelector
        onCheckedButtonChanged: {
            root.palette.highlight = checkedButton.color
        }
    }

    header: TitleBar {
        enableInWindowBlendBlur: true
        content: Item {
            SearchEdit {
                anchors.centerIn: parent
                width: 300
            }
        }
        menu: Menu {
            Action {
                text: "Light Theme"
            }
            Action {
                text: "Dark Theme"
            }
            MenuItem {
                contentItem: Item {
                    Row {
                        anchors.centerIn: parent
                        Repeater {
                            model: ["#d8316c", "#ff5d00", "#f8cb00", "#23c400", "#00a48a", "#0081ff", "#3c02d7", "#8c00d4"]
                            delegate: ColorButton {
                                color: modelData
                                Component.onCompleted: {
                                    activeColorSelector.addButton(this)
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    Popup {
        id: sourceViewerDialog
        width: parent.width
        height: parent.height
        modal: true

        enter: Transition {
            NumberAnimation { property: "opacity"; from: 0.0; to: 1.0 }
        }
        exit: Transition {
            NumberAnimation { property: "opacity"; from: 1.0; to: 0.0 }
        }

        Overlay.modal: Rectangle {
            color: palette.window
        }

        ViewQMLSource {
            id: sourceViewer
            anchors.fill: parent
        }
    }

    ListView {
        id: contentList
        anchors {
            fill: parent
            margins: 10
        }

        spacing: 10
        model: examplesFiles
        delegate: Column {
            width: contentList.width

            Loader {
                source: "qrc:/examples/" + modelData
                width: parent.width
            }
        }
        section {
            property: "modelData"
            delegate: GroupTitle {
                text: section
                width: parent.width
                onViewSource: {
                    sourceViewer.url = "qrc:/examples/" + section
                    sourceViewerDialog.open()
                }
            }
        }
    }
}
