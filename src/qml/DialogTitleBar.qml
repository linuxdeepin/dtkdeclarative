/*
 * Copyright (C) 2021 ~ 2022 UnionTech Technology Co., Ltd.
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
import QtQuick.Window 2.11
import QtQuick.Layouts 1.11
import org.deepin.dtk.impl 1.0 as D
import "PixelMetric.js" as PM

Control {
    id: control
    z: D.DTK.TopOrder
    width: Window.window.width
    height: PM.TitleBar_Height

    // custom control
    property alias content: customCenter.sourceComponent
    // dialog icon
    property alias icon: iconLabel
    property alias enableInWindowBlendBlur: background.visible

    property var __dwindow: Window.window.D.Window

    MouseArea {
        anchors.fill: parent
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
    }

    D.InWindowBlur {
        id: background
        anchors.fill: parent
        radius: 30
        visible: false
    }

    ColumnLayout {
        id: content
        spacing: 0
        anchors.fill: parent

        Rectangle {
            id: separatorTop
            width: control.width
            height: 1
            visible: !__dwindow.enabled
            color: "transparent"
        }

        RowLayout {
            spacing: 0
            Layout.alignment: Qt.AlignHCenter
            Layout.fillHeight: true
            D.DciIcon {
                id: iconLabel
                width: 32
                height: 32
                visible: name !== ""
                mode: control.D.ColorSelector.controlState
                theme: control.D.ColorSelector.controlTheme
                palette: D.DTK.makeIconPalette(control.palette)
            }

            // center custom area
            Loader {
                id: customCenter
                Layout.alignment: Qt.AlignHCenter
                Layout.fillHeight: true
                Layout.fillWidth: true
            }

            WindowButton {
                id: closeBtn
                icon.name: "window_close"
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
