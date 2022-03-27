/*
 * Copyright (C) 2022 UnionTech Technology Co., Ltd.
 *
 * Author:     JiDe Zhang <zhangjide@deepin.org>
 *
 * Maintainer: JiDe Zhang <zhangjide@deepin.org>
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

import QtQuick.Controls 2.4
import QtQuick 2.0
import QtQuick.Layouts 1.0

Item {
    property alias text: label.text
    signal viewSource

    implicitWidth: layout.implicitWidth
    implicitHeight: layout.implicitHeight + 20

    RowLayout {
        id: layout
        anchors.fill: parent
        spacing: 10

        Label {
            id: label
            Layout.alignment: Qt.AlignVCenter
        }
        Rectangle {
            id: line
            color: Qt.rgba(0, 0, 0, 0.08)
            height: 1
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignVCenter
        }
        Button {
            id: button
            text: "查看源码"
            Layout.alignment: Qt.AlignVCenter
            onClicked: viewSource()
        }
    }
}
