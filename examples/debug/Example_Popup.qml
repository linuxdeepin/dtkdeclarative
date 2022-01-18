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
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.11
import QtQuick.Window 2.11
import org.deepin.dtk 1.0 as D

Column {
    id: control

    Row {
        Button {
            text: "popup window"
            onClicked: {
    //            popup.x = (Screen.desktopAvailableWidth - Window.window.width) / 2
    //            popup.y = (Screen.desktopAvailableHeight - Window.window.height) / 2

    //            popupWindow.width = 400
                popupWindow.open()
    //            popupWindow.D.PopupWindow.window.hide()
    //            Window.window.hide()
    //            popup.D.PopupWindow.window.show()
    //            popupWindow.D.PopupWindow.close()
            }
    //        Component.onCompleted: popupWindow.open()
        }
        Button {
            text: "change x"
            onClicked: {
                popupWindow.x = 500
                popupWindow.open()
            }
        }
        Button {
            text: "change width"
            onClicked: {
                popupWindow.width = 400
                popupWindow.open()
            }
        }

        Button {
            text: "popup control"
            onClicked: {
    //            popupWindow.x = (Screen.desktopAvailableWidth - Window.window.width) / 2
    //            popupWindow.y = (Screen.desktopAvailableHeight - Window.window.height) / 2

                popup.open()
            }
        }
    }

    Row {
        Button {
            text: "menu control"
            onClicked: {
                menu.open()
            }
        }

        Button {
            text: "menu popupWindow"
            onClicked: {
                menuPopup.open()
            }
        }
    }
    Button {
        text: "handle forceWindowMode"
        onClicked: {
            popupWindow.D.PopupHandle.forceWindowMode = !popupWindow.D.PopupHandle.forceWindowMode
        }
    }

    D.Popup {
        id: popupWindow; objectName: "pupup window"
        visible: false
        x: 100
        y: 20
//        width: 200
//        height: 100
//        width: 300
//        height: 300
//        margins: 100
        D.PopupHandle.forceWindowMode: true
        D.PopupHandle.delegate: D.PopupWindow {
            blurControl: popupWindow
        }
        contentItem: Column {
            spacing: 10
            Text {
                text: "蓝牙"
                anchors.horizontalCenter: parent.horizontalCenter
            }
            Button {
                text: "打印机"
                anchors.horizontalCenter: parent.horizontalCenter
            }
        }
    }
    D.Popup {
        id: popup;  objectName: "pupup"
        x: 100
        y: 20
//        margins: 100
        contentItem: Column {
            spacing: 10
            Text {
                text: "蓝牙"
                anchors.horizontalCenter: parent.horizontalCenter
            }
            Button {
                text: "打印机"
                anchors.horizontalCenter: parent.horizontalCenter
            }
        }
    }
    Menu {
        id: menu
        MenuItem { text: "Text" }
    }
    Menu {
        id: menuPopup
        MenuItem { text: "Text" }

        D.PopupHandle.forceWindowMode: true
    }
}
