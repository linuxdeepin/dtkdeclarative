/*
 * Copyright (C) 2022 UnionTech Technology Co., Ltd.
 *
 * Author:     Chen Bin <chenbin@uniontech.com>
 *
 * Maintainer: Chen Bin <chenbin@uniontech.com>
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
import org.deepin.dtk 1.0 as D

Rectangle {
    Label {
        id: changePaletteLabel
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.topMargin: 20
        text: "Change Palette By object property"
        font.pointSize: 14
        font.bold: true
    }

    Control {
        id: control1
        anchors.left: parent.left
        anchors.top: changePaletteLabel.bottom
        anchors.topMargin: 20
        width: 500
        height: 50

        property D.Palette backgroundColor: D.Palette {
            normal: "black"
        }
        property D.Palette backgroundColorClicked: D.Palette {
            normal: "blue"
        }

        property D.Palette textColorClicked: D.Palette {
            normal: "yellow"
        }

        Rectangle {
            property D.Palette borderColor: D.Palette {
                normal: "red"
            }
            id: rect1
            anchors.left: parent.left
            anchors.top: control1.top
            width: 250
            height: 50
            color: D.ColorSelector.backgroundColor
            border.width: 1
            border.color: D.ColorSelector.borderColor

            Text {
                property D.Palette textColor: D.Palette {
                    normal: "white"
                }

                id: rect1Text
                anchors.fill: parent
                text: "Use it's control parent's palette.\n(Click to change.)"
                color: D.ColorSelector.textColor
                horizontalAlignment: Qt.AlignHCenter
                verticalAlignment: Qt.AlignVCenter

                MouseArea {
                    anchors.fill: parent

                    onClicked: {
                        control1.backgroundColor.normal = "red"
                        rect1Text.textColor.normal = "black"
                    }
                }
            }
        }

        Rectangle {
            id: rect2
            property D.Palette borderColor: D.Palette {
                normal: "red"
            }
            property D.Palette backgroundColor: D.Palette {
                normal: "black"
            }
            property D.Palette backgroundColorClicked: D.Palette {
                normal: "magenta"
            }

            anchors.left: rect1.right
            anchors.leftMargin: 20
            anchors.top: control1.top
            width: 250
            height: 50
            color: D.ColorSelector.backgroundColor
            border.width: 1
            border.color: D.ColorSelector.borderColor

            Text {
                id: rect2Text
                property D.Palette textColor: D.Palette {
                    normal: "yellow"
                }

                property D.Palette textColorClicked: D.Palette {
                    normal: "blue"
                }

                anchors.fill: parent
                text: "Overwrite it's control parent palette.\n(Click to change.)"
                color: D.ColorSelector.textColor
                horizontalAlignment: Qt.AlignHCenter
                verticalAlignment: Qt.AlignVCenter

                MouseArea {
                    anchors.fill: parent
                    D.Palette {
                        id: otherBackgroundColor
                        normal: "darkRed"
                    }
                    D.Palette {
                        id: othertextColor
                        normal: "darkBlue"
                    }

                    onClicked: {
                        rect2.backgroundColor = otherBackgroundColor
                        rect2Text.textColor = othertextColor
                    }
                }
            }
        }
    }

    Label {
        id: changeObjectLabel
        anchors.left: parent.left
        anchors.top: control1.bottom
        anchors.topMargin: 20
        text: "Change Palette By color selector"
        font.pointSize: 14
        font.bold: true
    }

    Control {
        id: control2
        anchors.left: parent.left
        anchors.top: changeObjectLabel.bottom
        anchors.topMargin: 20
        width: 500
        height: 50

        property D.Palette backgroundColor: D.Palette {
            normal: "black"
        }
        property D.Palette backgroundColorClicked: D.Palette {
            normal: "blue"
        }

        property D.Palette textColorClicked: D.Palette {
            normal: "cyan"
        }

        Rectangle {
            property D.Palette borderColor: D.Palette {
                normal: "red"
            }
            id: rect3
            anchors.left: parent.left
            anchors.top: control2.top
            width: 250
            height: 50
            color: D.ColorSelector.backgroundColor
            border.width: 1
            border.color: D.ColorSelector.borderColor

            Text {
                property D.Palette textColor: D.Palette {
                    normal: "white"
                }
                id: rect3Text
                anchors.fill: parent
                text: "Use it's color seletor to change palette.\n(Click to change, double click restore)"
                color: D.ColorSelector.textColor
                horizontalAlignment: Qt.AlignHCenter
                verticalAlignment: Qt.AlignVCenter

                MouseArea {
                    anchors.fill: parent

                    onClicked: {
                        rect3.D.ColorSelector.backgroundColor = control2.backgroundColorClicked
                        rect3Text.D.ColorSelector.textColor = control2.textColorClicked
                    }

                    onDoubleClicked: {
                        rect3.D.ColorSelector.backgroundColor = undefined
                        rect3Text.D.ColorSelector.textColor = undefined
                    }
                }
            }
        }

        Rectangle {
            id: rect4
            anchors.left: rect3.right
            anchors.top: control2.top
            anchors.leftMargin: 20
            width: 250
            height: 50
            color: D.ColorSelector.backgroundColor
            property D.Palette backgroundColor: D.Palette {
                normal: "black"
            }
            property D.Palette backgroundColorClicked: D.Palette {
                normal: "magenta"
            }

            Text {
                id: rect4Text
                property D.Palette textColor: D.Palette {
                    normal: "yellow"
                }

                property D.Palette textColorClicked: D.Palette {
                    normal: "blue"
                }

                anchors.fill: parent
                text: "Use CS to replace it's own palette.\n(Click to change, double click restore)"
                color: D.ColorSelector.textColor
                horizontalAlignment: Qt.AlignHCenter
                verticalAlignment: Qt.AlignVCenter

                MouseArea {
                    anchors.fill: parent

                    onClicked: {
                        rect4.D.ColorSelector.backgroundColor = rect4.backgroundColorClicked
                        rect4Text.D.ColorSelector.textColor = control2  .textColorClicked
                    }

                    onDoubleClicked: {
                        rect4.D.ColorSelector.backgroundColor = undefined
                        rect4Text.D.ColorSelector.textColor = undefined
                    }
                }
            }
        }

        Label {
            id: changeParentLabel
            anchors.left: parent.left
            anchors.top: control2.bottom
            anchors.topMargin: 20
            text: "Change palette when parent changed"
            font.pointSize: 14
            font.bold: true
        }

        Rectangle {
            id: changeParentRect
            anchors.left: parent.left
            anchors.top: changeParentLabel.bottom
            anchors.topMargin: 20
            width: 500
            height: 50

            Control {
                id: control3
                width: 220
                height: 50
                anchors.left: parent.left
                anchors.top: parent.top
                property D.Palette backgroundColor: D.Palette {
                    normal: "gray"
                }

                Rectangle {
                    property D.Palette borderColor: D.Palette {
                        normal: "red"
                    }
                    id: reparentRect1
                    width: 50
                    height: 50
                    color: D.ColorSelector.backgroundColor
                    border.width: 1
                    border.color: D.ColorSelector.borderColor

                    Text {
                        property D.Palette textColor: D.Palette {
                            normal: "blue"
                        }
                        anchors.fill: parent
                        text: "Rect1"
                        color: D.ColorSelector.textColor
                        horizontalAlignment: Qt.AlignHCenter
                        verticalAlignment: Qt.AlignVCenter

                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                if (reparentRect1.parent == control3)
                                    reparentRect1.state = "reparent2"
                                else
                                    reparentRect1.state = "reparent1"
                            }
                        }
                    }

                    states: [
                        State {
                            name: "reparent1"
                            ParentChange {
                                target: reparentRect1
                                parent: control3
                                x: 0
                                y: 0
                            }
                        },
                        State {
                            name: "reparent2"
                            ParentChange {
                                target: reparentRect1
                                parent: control4
                                x: 60
                                y: 0
                            }
                        }
                    ]
                }
            }

            Control {
                id: control4
                width: 220
                height: 50
                anchors.left: control3.right
                anchors.leftMargin: 10
                anchors.top: parent.top

                property D.Palette backgroundColor: D.Palette {
                    normal: "red"
                }

                Rectangle {
                    property D.Palette borderColor: D.Palette {
                        normal: "gray"
                    }
                    id: reparentRect2
                    width: 50
                    height: 50
                    color: D.ColorSelector.backgroundColor
                    border.width: 1
                    border.color: D.ColorSelector.borderColor

                    Text {
                        property D.Palette textColor: D.Palette {
                            normal: "yellow"
                        }
                        anchors.fill: parent
                        text: "Rect2"
                        color: D.ColorSelector.textColor
                        horizontalAlignment: Qt.AlignHCenter
                        verticalAlignment: Qt.AlignVCenter

                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                if (reparentRect2.parent == control3)
                                    reparentRect2.state = "reparent2"
                                else
                                    reparentRect2.state = "reparent1"
                            }
                        }
                    }

                    states: [
                        State {
                            name: "reparent1"
                            ParentChange {
                                target: reparentRect2
                                parent: control3
                                x: 60
                                y: 0
                            }
                        },
                        State {
                            name: "reparent2"
                            ParentChange {
                                target: reparentRect2
                                parent: control4
                                x: 0
                                y: 0
                            }
                        }
                    ]
                }
            }
        }

        Label {
            id: customControlLabel
            anchors.left: parent.left
            anchors.top: changeParentRect.bottom
            anchors.topMargin: 20
            text: "Custom control which used the color selector"
            font.pointSize: 14
            font.bold: true
        }

        Row {
            id: dontOverwriteControls
            width: parent.width
            height: 50
            anchors.left: parent.left
            anchors.top: customControlLabel.bottom
            anchors.topMargin: 20
            spacing: 5

            Repeater {
                model: 5
                Example_customcontrolforcs {
                    width: 50
                    height: 50
                    text: modelData
                }
            }
        }

        Row {
            id: overwriteControls
            width: parent.width
            height: 50
            anchors.left: parent.left
            anchors.top: dontOverwriteControls.bottom
            anchors.topMargin: 20
            spacing: 5

            Repeater {
                model: 5
                Example_customcontrolforcs {
                    backgroundColor: D.Palette {
                        normal: Qt.hsla(Math.random(), 0.6, 0.6, 0.9)
                        hovered: Qt.hsla(Math.random(), 0.2, 0.2, 0.9)
                    }

                    width: 50
                    height: 50
                    text: modelData
                }
            }
        }
    }
}
