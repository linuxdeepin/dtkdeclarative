/*
 * Copyright (C) 2021 UnionTech Technology Co., Ltd.
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
import QtQuick.Controls 2.4
import QtQuick.Window 2.11
import QtQuick.Layouts 1.11
import org.deepin.dtk.impl 1.0 as D

MouseArea {
    id: control
    width: Window.window.width
    height: 45

    // custom control
    property alias content: customCenter.sourceComponent
    // dialog icon
    property alias iconName: iconLabel.icon.name

    property var __dwindow: Window.window.D.Window

    acceptedButtons: Qt.AllButtons
    propagateComposedEvents: true
    onPressed: {
        if (mouse.button === Qt.RightButton) {
            if (mouse.x < control.width - closeBtn.width) {
                __dwindow.popupSystemWindowMenu()
                mouse.accepted = true
                return
            }
        }
        mouse.accepted = false
    }

    ColumnLayout {
        id: content
        spacing: 0
        anchors.fill: parent

        Rectangle {
            id: separatorTop
            width: control.width
            height: 1
            color: "transparent"
        }

        RowLayout {
            spacing: 0
            Layout.alignment: Qt.AlignHCenter
            Layout.fillHeight: true
            Button {
                id: iconLabel
                width: 32
                height: 32
                visible: control.iconName !== ""
            }

            // center custom area
            Loader {
                id: customCenter
                Layout.alignment: Qt.AlignHCenter
                Layout.fillHeight: true
                Layout.fillWidth: true
            }

            WindowCloseButton {
                id: closeBtn
                property bool hasWindowFlag/*: (Window.window.flags & Qt.WindowCloseButtonHint)*/
                Component.onCompleted: hasWindowFlag = (Window.window.flags & Qt.WindowCloseButtonHint)

                visible: hasWindowFlag && __dwindow.enabled

                enabled: __dwindow.motifFunctions & D.WindowManagerHelper.FUNC_CLOSE
                onClicked: Window.window.close()
            }
        }

        Rectangle {
            id: separatorButtom
            width: control.width
            height: 1
            color: "transparent"
            Layout.alignment: Qt.AlignBottom
        }
    }
}
