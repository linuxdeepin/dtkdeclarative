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
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

import QtQuick 2.11
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.11
import org.deepin.dtk.impl 1.0 as D
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
            name: "view-close"
            width: DS.Style.floatingMessage.closeButtonSize
            height: DS.Style.floatingMessage.closeButtonSize
        }
        onClicked: D.DTK.closeMessage(control)
    }

    duration: 4000
    panel: FloatingPanel {
        id: floatingPanel
        leftPadding: 10
        rightPadding: 10
        topPadding: 0
        bottomPadding: 0

        contentItem: RowLayout {
            height: DS.Style.floatingMessage.minimumHeight
            width: Math.min(DS.Style.floatingMessage.maximumWidth, children.width + floatingPanel.leftPadding - floatingPanel.rightPadding)
            spacing: 10

            D.DciIcon {
                id: iconLoader
                sourceSize {
                    width: 20
                    height: 20
                }
                Layout.alignment: Qt.AlignVCenter
                name: control.message.iconName || ""
                visible: name != ""
            }

            Loader {
                id: contentLoader
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.alignment: Qt.AlignVCenter
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
        }
    }
}