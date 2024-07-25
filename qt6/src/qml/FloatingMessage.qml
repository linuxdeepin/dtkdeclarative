// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

import QtQuick 2.11
import QtQuick.Layouts 1.11
import org.deepin.dtk 1.0 as D
import org.deepin.dtk.style 1.0 as DS

D.FloatingMessageContainer {
    id: control

    property Component contentItem: Label {
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignLeft
        text: control.message.content || ""
        elide: Text.ElideRight
        maximumLineCount: 1
    }
    property Component button: ActionButton {
        icon {
            name: "entry_clear"
            width: DS.Style.floatingMessage.closeButtonSize
            height: DS.Style.floatingMessage.closeButtonSize
        }
        onClicked: {
             floatingPanel.state = "small"
             destroyTimer.running = true
        }
        Timer {
           id: destroyTimer
           interval: 1200; running: false; repeat: true
           onTriggered: D.DTK.closeMessage(control)
        }
    }


    duration: 4000
    panel: FloatingPanel {
        id: floatingPanel
        implicitWidth: DS.Style.control.contentImplicitWidth(floatingPanel)
        leftPadding: 10
        rightPadding: 10
        topPadding: 0
        bottomPadding: 0
        opacity: 0.0
        state: "small"
        Timer {
            id: timer
            interval: 100; running: false; repeat: false
            onTriggered: {
                floatingPanel.y = floatingPanel.parent.height
                floatingPanel.state = "normal"
                console.log("onCompleted", floatingPanel.y, floatingPanel.parent.width, floatingPanel.parent.height)
            }
        }
        Component.onCompleted: {
            timer.running = true
        }
        contentItem: RowLayout {
            height: DS.Style.floatingMessage.minimumHeight
            width: Math.min(DS.Style.floatingMessage.maximumWidth, children.width + floatingPanel.leftPadding - floatingPanel.rightPadding)
            spacing: 10
            Loader {
                id: iconLoader
                Layout.alignment: Qt.AlignVCenter
                property string iconName: control.message.iconName || ""
                active: iconName

                sourceComponent: D.DciIcon {
                    sourceSize {
                        width: 20
                        height: 20
                    }
                    name: iconName
                    mode: floatingPanel.D.ColorSelector.controlState
                    theme: floatingPanel.D.ColorSelector.controlTheme
                    palette: D.DTK.makeIconPalette(floatingPanel.palette)
                }
            }

            Loader {
                id: contentLoader
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.alignment: Qt.AlignVCenter
                Layout.rightMargin: closeButton.active ? 0 : 6 // when no closeButton, contentLoader.rightMargin is 10 + 6, otherwise is 10
                Layout.maximumWidth: maxContentWidth
                Layout.preferredHeight: DS.Style.floatingMessage.minimumHeight
                property int maxContentWidth: DS.Style.floatingMessage.maximumWidth - iconLoader.implicitWidth - closeButton.implicitWidth
                sourceComponent: control.contentItem
            }

            Loader {
                id: closeButton
                Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
                active: control.duration < 0
                visible: active
                sourceComponent: button
            }

            ParallelAnimation {
                running: closeButton.item.hovered
                NumberAnimation { target: closeButton; property: "scale"; to: 1.25; duration: 500 }
                NumberAnimation { target: closeButton; property: "rotation"; to: 90; duration: 500 }
            }
            ParallelAnimation {
                running: !closeButton.item.hovered
                NumberAnimation { target: closeButton; property: "scale"; to: 1; duration: 500 }
                NumberAnimation { target: closeButton; property: "rotation"; to: 0; duration: 500 }
            }
        }

        states: [
            State {
                name: "normal"
                PropertyChanges {
                    target: floatingPanel
                    y: 0
                    opacity: 1.0
                    scale: 1.0
                }
            },
            State {
                name: "small"
                PropertyChanges {
                    target: floatingPanel
                    y: floatingPanel.parent.height
                    opacity: 0.0
                    scale: 0.2
                }
            }
        ]

        transitions: Transition {
            NumberAnimation {
                properties: "y, opacity, scale"
                easing.type: Easing.Linear
                duration: 1000
            }
         }
    }
}
