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
import "PixelMetric.js" as PM

MouseArea {
    id: control
    z: D.DTK.TopOrder
    width: Window.window.width
    // it's binding `height` instead of `visible` property,
    // because MouseArea should accept event keeping visible.
    height: (!__isFullScreen || __isVisible) ? PM.TitleBar_Height : 0
    hoverEnabled: __isFullScreen && autoHideOnFullscreen
    acceptedButtons: Qt.AllButtons
    propagateComposedEvents: true

    property string title: Window.window.title
    property alias icon: iconLabel

    // custom control
    property alias content: customCenter.sourceComponent

    // menu access
    property alias menu: optionMenuLoader.sourceComponent
    property bool menuDisabled
    property Component aboutDialog

    // visibility access
    property bool fullScreenButtonVisible: true
    signal toggleWindowState()
    property alias windowBuggonGroup: windowButtonsLoader.sourceComponent

    property bool autoHideOnFullscreen: false
    property bool embedMode: !__dwindow.enabled
    property bool separatorVisible: true

    property var __dwindow: Window.window.D.Window
    property bool __isFullScreen: Window.window.visibility === Window.FullScreen
    property bool __isVisible: control.containsMouse
    readonly property int __includedAreaX: control.width - optionMenuBtn.width - windowButtonsLoader.width

    property alias enableInWindowBlendBlur: background.visible

    onPressed: {
        if (mouse.button === Qt.RightButton) {
            if (mouse.x < __includedAreaX) {
                __dwindow.popupSystemWindowMenu()
                mouse.accepted = true
                return
            }
        }
        mouse.accepted = false
    }
    onDoubleClicked: {
        // Windowed or Maximized
        if (mouse.button == Qt.LeftButton) {
            control.toggleWindowState()
            mouse.accepted = true
            return
        }
        mouse.accepted = false
    }
    onReleased: mouse.accepted = false
    onClicked: mouse.accepted = false

    D.InWindowBlendBlur {
        id: background
        anchors.fill: parent
        radius: 30
        visible: false
    }

    ColumnLayout {
        id: content
        spacing: 0
        anchors.fill: parent
        visible: control.height > 0

        Rectangle {
            id: separatorTop
            width: control.width
            height: 1
            visible: embedMode
            color: "transparent"
        }

        RowLayout {
            spacing: 0
            Layout.alignment: Qt.AlignHCenter
            Layout.fillHeight: true

            D.DciIcon {
                id: iconLabel
                sourceSize.width: 32
                sourceSize.height: 32
                Layout.alignment: Qt.AlignLeft
                visible: control.iconName !== ""
            }

            // center custom area
            Loader {
                id: customCenter
                height: parent.height
                Layout.alignment: Qt.AlignHCenter
                Layout.fillWidth: true
                sourceComponent: titleCenterCom
            }

            // option button, it maybe load other Button in different scene.
            WindowButton {
                id: optionMenuBtn
                icon.name: "window_menu"
                enabled: !menuDisabled
                Layout.alignment: Qt.AlignRight
                onClicked: {
                    var pos = Qt.point(optionMenuLoader.x, control.height)
                    optionMenuLoader.item.popup(optionMenuBtn, pos)
                }

                Loader {
                    id: optionMenuLoader
                    sourceComponent: Menu {
                        ThemeMenu{}
                        MenuSeparator {
                            contentItem: Rectangle {
                                implicitHeight: 1
                                color: "black"
                            }
                        }
                        HelpAction {}
                        AboutAction { aboutDialog: control.aboutDialog }
                        QuitAction{}
                    }
                    onLoaded: {
                        for (var i = 0; i < item.count; i++) {
                            var action = item.actionAt(i)
                            if (action && action.objectName === "_d_about_action") {
                                if (!action.aboutDialog) {
                                    action.aboutDialog = control.aboutDialog
                                    break
                                }
                            }
                        }
                    }
                }
            }

            Loader {
                id: windowButtonsLoader
                sourceComponent: WindowButtonGroup {
                    Layout.alignment: Qt.AlignRight
                    embedMode: control.embedMode
                    fullScreenButtonVisible: control.fullScreenButtonVisible
                    Component.onCompleted: {
                        control.toggleWindowState.connect(maxOrWinded)
                    }
                }
            }
        }

        Rectangle {
            id: separatorButtom
            width: control.width
            height: 1
            visible: separatorVisible
            color: "transparent"
            Layout.alignment: Qt.AlignBottom
        }
    }

    Component {
        id: titleCenterCom
        Label {
            textFormat: Text.PlainText
            property bool hasWindowFlag/*: (Window.window.flags & Qt.WindowTitleHint)*/
            Component.onCompleted: hasWindowFlag = (Window.window.flags & Qt.WindowTitleHint)

            visible: hasWindowFlag && !embedMode &&
                        (__dwindow.motifDecorations & D.WindowManagerHelper.DECOR_TITLE)

            text: control.title
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }
    }
}
