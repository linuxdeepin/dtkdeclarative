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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

import QtQuick 2.0
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.11
import org.deepin.dtk 1.0 as D

Flow {
    id: control

    ScrollView {
        id: scrollView
        width: 200
        height: 400
        clip: true
        ListView {
            model: 20
            delegate: Button {
                text: "index" + String(modelData)
            }
        }
        background: Rectangle {
            border.color: "green"
        }
    }
    Column {
        spacing: 10
        D.IpV4LineEdit {

        }
        D.IpV4LineEdit {
            width: 300
            height: 40
            showAlert: focus
            alertText: "alert tips"
        }
        D.IpV4LineEdit {
            width: 300
            height: 40
            text: "10.20.52.57"
        }

        Row {
            spacing: 10
            D.IpV4LineEdit {
                id: idLineEditSetValueByText
                width: 300
                height: 40
            }

            Button {
                text: "set IP by Text"
                onClicked: idLineEditSetValueByText.text = "10.20.52.57"
            }
            Text {
                text: idLineEditSetValueByText.text
            }
        }
    }

    Column {
        width: 340
        height: 280

        RowLayout {
            TextField {
                id: nameFilter
                placeholderText: qsTr("Search by name...")
                Layout.fillWidth: true
                onTextChanged: sortFilterModel.update()
            }
            RadioButton {
                id: sortByName
                checked: true
                text: qsTr("Sort by name")
                onCheckedChanged: sortFilterModel.update()
            }
            RadioButton {
                text: qsTr("Sort by team")
                onCheckedChanged: sortFilterModel.update()
            }
        }

        D.SortFilterModel {
            id: sortFilterModel
            model: ListModel {
                ListElement { name: "Alice"; team: "Crypto" }
                ListElement { name: "Bob"; team: "Crypto" }
                ListElement { name: "Jane"; team: "QA" }
                ListElement { name: "Victor"; team: "QA" }
                ListElement { name: "Wendy"; team: "Graphics" }
            }
            delegate: Text {
                text: name + " (" + team + ")"
            }
            filterAcceptsItem: function(item) {
                return item.name.includes(nameFilter.text)
            }
            lessThan: function(left, right) {
                var leftVal = sortByName.checked ? left.name : left.team;
                var rightVal = sortByName.checked ? right.name : right.team;
                return leftVal < rightVal ? -1 : 1;
            }
        }

        ListView {
            height: 100
            width: parent.width
            model: sortFilterModel
        }
    }
}
