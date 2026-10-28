// SPDX-FileCopyrightText: 2020 - 2026 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

import QtQuick
import QtQuick.Layouts
import org.deepin.dtk.style 1.0 as DS

FocusScope {
    id: control
    // Default SpinBox Properties
    property alias spinBox: spinBoxControl
    // Button Visible Properties
    property alias upButtonVisible: plusButton.active
    property alias downButtonVisible: minusButton.active
    property alias resetButtonVisible: resetButton.active

    implicitWidth: DS.Style.spinBox.width
    implicitHeight: DS.Style.spinBox.height

    RowLayout {
        anchors.fill: parent
        spacing: DS.Style.plusMinusSpinBox.spacing
        SpinBox {
            id: spinBoxControl
            Layout.fillWidth: true
            up.indicator: null
            down.indicator: null
            rightPadding: clearBtn.active ? (clearBtn.width + clearBtn.anchors.rightMargin) : 0

            Loader {
                id: clearBtn
                active: spinBoxControl.displayText.length !== 0
                width: height
                height: spinBoxControl.height
                anchors {
                    right: spinBoxControl.right
                    verticalCenter: spinBoxControl.verticalCenter
                }

                sourceComponent: ActionButton {
                    focusPolicy: Qt.NoFocus
                    icon {
                        name: "entry_clear"
                        width: DS.Style.edit.actionIconSize
                        height: DS.Style.edit.actionIconSize
                    }
                    onClicked: spinBoxControl.value = spinBoxControl.from
                }
            }
        }

        Row {
            spacing: DS.Style.plusMinusSpinBox.buttonSpacing

            Loader {
                id: plusButton
                active: true
                sourceComponent: ToolButton {
                    implicitWidth: spinBoxControl.height
                    implicitHeight: spinBoxControl.height
                    focusPolicy: Qt.NoFocus
                    icon.name: "action_add"
                    icon.width: 16
                    icon.height: 16
                    onClicked: spinBoxControl.increase()
                }
            }

            Loader {
                id: minusButton
                active: true
                sourceComponent: ToolButton {
                    implicitWidth: spinBoxControl.height
                    implicitHeight: spinBoxControl.height
                    focusPolicy: Qt.NoFocus
                    icon.name: "action_reduce"
                    icon.width: 16
                    icon.height: 16
                    onClicked: spinBoxControl.decrease()
                }
            }

            Loader {
                id: resetButton
                active: false
                sourceComponent: ToolButton {
                    implicitWidth: spinBoxControl.height
                    implicitHeight: spinBoxControl.height
                    focusPolicy: Qt.NoFocus
                    icon.name: "action_reset"
                    icon.width: 16
                    icon.height: 16
                    onClicked: { spinBoxControl.value = 0 }
                }
            }
        }
    }
}
