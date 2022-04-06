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
import QtQuick.Layouts 1.11
import org.deepin.dtk 1.0 as D
import org.deepin.dtk.style 1.0 as DS
import "private"

Control {
    id: control
    property string text
    property string placeholderText
    property alias keys: listener.keys
    property D.Palette backgroundColor: DS.Style.keySequenceEdit.background
    property D.Palette placeholderTextColor: DS.Style.keySequenceEdit.placeholderText

    background: Rectangle {
        implicitWidth: DS.Style.keySequenceEdit.width
        implicitHeight: DS.Style.keySequenceEdit.height
        radius: DS.Style.control.radius
        color: control.D.ColorSelector.backgroundColor
    }

    contentItem: RowLayout {
        Label {
            Layout.leftMargin: DS.Style.keySequenceEdit.margin
            text: control.text
            horizontalAlignment: Qt.AlignLeft
            verticalAlignment: Qt.AlignVCenter
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.alignment: Qt.AlignLeft | Qt.AlignVCenter
        }

        Component {
            id: inputComponent
            Label {
                text: control.placeholderText
                color: control.D.ColorSelector.placeholderTextColor
                font: D.DTK.fontManager.t7
                horizontalAlignment: Qt.AlignRight
                verticalAlignment: Qt.AlignVCenter
            }
        }

        Component {
            id: keyComponent
            RowLayout {
                spacing: DS.Style.keySequenceEdit.margin
                Repeater {
                    model: control.keys
                    KeySequenceLabel {
                        Layout.alignment: Qt.AlignRight
                        text: modelData
                    }
                }
            }
        }

        Loader {
            Layout.rightMargin: DS.Style.keySequenceEdit.margin
            Layout.alignment: Qt.AlignVCenter
            sourceComponent: (control.keys.length !== 0) ? keyComponent : inputComponent

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    control.forceActiveFocus()
                    listener.clearKeys()
                }
            }
        }
    }

    D.KeySequenceListener {
        id: listener
        target: control
    }
}
