// SPDX-FileCopyrightText: 2024 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

import QtQuick 2.0
import QtQuick.Layouts 1.11
import org.deepin.dtk 1.0 as D
import org.deepin.dtk.style 1.0 as DS

ColumnLayout {
    id: root
    spacing: 10
    clip: true

    property string title
    property bool isExpanded: true
    property int interval: 400 / (itemLayout.children.length)
    property int index: 0
    property int titleHeight: 36
    default property alias childItem: itemLayout.children

    Control {
        id: title
        Layout.fillWidth: true
        Layout.preferredHeight: 36
        property int inset: 4
        leftInset: inset
        rightInset: inset
        topInset: inset
        bottomInset: inset
        state: "normal"
        states: [
            State {
                name: "normal"
                PropertyChanges {
                    target: title
                    inset:4
                }
            },
            State {
                name: "hovered"
                PropertyChanges {
                    target: title
                    inset:0
                }
            },
            State {
                name: "pressed"
                PropertyChanges {
                    target: title
                    inset: 4
                }
            }
        ]
        transitions: [
            Transition {
                from: "normal"
                to: "hovered"
                NumberAnimation {
                    properties: "inset"
                    easing.type: Easing.Linear
                    duration: 500
                }
            },
            Transition {
                from: "hovered"
                to: "normal"
                NumberAnimation {
                    properties: "inset"
                    easing.type: Easing.Linear
                    duration: 500
                }
            },
            Transition {
                from: "hovered"
                to: "pressed"
                NumberAnimation {
                    properties: "inset"
                    easing.type: Easing.Linear
                    duration: 500
                }
            },
            Transition {
                from: "pressed"
                to: "hovered"
                NumberAnimation {
                    properties: "inset"
                    easing.type: Easing.Linear
                    duration: 500
                }
            }
        ]
        RowLayout {
            anchors.fill: parent
            spacing: 0
            Label {
                Layout.fillWidth: true
                Layout.fillHeight: true
                text: root.title
                font: DTK.fontManager.t5
                verticalAlignment: Qt.AlignVCenter
            }
            D.DciIcon {
                Layout.preferredWidth: 36
                Layout.preferredHeight: 36
                rotation: root.isExpanded ? 0 : - 90
                name: "arrow_ordinary_down"

                Behavior on rotation {
                    NumberAnimation {
                        duration: root.interval
                        easing.type: Easing.Linear
                    }
                }
            }
        }
        MouseArea {
            id: mouseArea
            anchors.fill: parent
            hoverEnabled: true
            onClicked: root.isExpanded = !root.isExpanded
            onPressed: title.state = "pressed"
            onReleased: title.state = "hovered"
            onEntered: title.state = "hovered"
            onExited: title.state = "normal"
        }
        background: Item {
            Loader {
                anchors.fill: parent
                active: title.hovered
                sourceComponent: D.RoundRectangle {
                    color: DS.Style.itemDelegate.normalColor
                    radius: DS.Style.control.radius
                    corners: D.RoundRectangle.TopLeftCorner | D.RoundRectangle.TopRightCorner | D.RoundRectangle.BottomLeftCorner | D.RoundRectangle.BottomRightCorner
                }
            }
        }
    }

    Timer {
        id: timer
    }

    ColumnLayout {
        id: itemLayout
    }

    onIsExpandedChanged: (isExpanded) => {
        for(let i = 0; i < itemLayout.children.length; ++i) {
            itemLayout.children[i].isExpanded = !itemLayout.children[i].isExpanded
        }
        //     delay(0, timeout)
    }

    function timeout() {
        itemLayout.children[index].isExpanded = !itemLayout.children[index].isExpanded
        ++index
        if (index === itemLayout.children.length) {
            timer.stop()
            timer.triggered.disconnect(timeout)
            index = 0
        }
    }

    function delay(delayTime, cb) {
        timer.interval = delayTime
        timer.repeat = true
        timer.triggered.connect(cb)
        timer.restart()
    }
}
