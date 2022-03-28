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

import QtQuick 2.0
import QtQuick.Layouts 1.11
import QtQuick.Controls 2.4
import QtQml.Models 2.11
import org.deepin.dtk 1.0

ColumnLayout {
    id: control
    property string title
    property alias model: model.model
    property alias view: info
    property Component action
    signal clicked(string key)
    property int itemSpacing: 20
    property font itemFont: DTK.fontManager.t5
    property ButtonGroup group: ButtonGroup {}

    width: 220
    spacing: 20
    RowLayout {
        Layout.preferredHeight: 24
        Layout.preferredWidth: 200
        Label {
            text: title
            font: DTK.fontManager.t5
            Layout.fillWidth: true; Layout.preferredHeight: 24
        }
        Loader {
            sourceComponent: control.action
            Layout.alignment: Qt.AlignRight
        }
    }
    ListView {
        id: info
        spacing: control.itemSpacing
        Layout.preferredHeight: contentHeight
        Layout.fillWidth: true
        model: DelegateModel {
            id: model
            delegate: ItemDelegate {
                width: 200
                icon.name: model.icon
                text: model.text
                checked: model.checked
                backgroundVisible: false
                font: control.itemFont
                ButtonGroup.group: group
                onClicked: control.clicked(model.index)
            }
        }
    }
}
