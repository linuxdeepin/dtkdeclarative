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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

import QtQuick 2.11
import QtQuick.Controls 2.4
import org.deepin.dtk.impl 1.0 as D
import org.deepin.dtk.settings 1.0 as Settings
import org.deepin.dtk.style 1.0 as DS
import ".."

Control {
    id: control
    signal clicked()
    property bool checked: false
    property D.Palette backgroundColor: DS.Style.settings.background
    property D.Palette checkedTextColor: DS.Style.checkedButtonText

    palette.windowText: checked ? D.ColorSelector.checkedTextColor : undefined
    contentItem: Label {
        text: Settings.SettingsGroup.name
        font: __getFont(Settings.SettingsGroup.level)
        leftPadding: __getMargin(Settings.SettingsGroup.level)
        topPadding: DS.Style.settings.navigationTextVPadding
        bottomPadding: DS.Style.settings.navigationTextVPadding
        anchors.left: parent.left
        verticalAlignment: Qt.AlignVCenter
        elide: Text.ElideRight

        function __getFont(level) {
            switch(level) {
            case 0:
                return D.DTK.fontManager.t4
            case 1:
                return D.DTK.fontManager.t5
            }
            return D.DTK.fontManager.t6
        }
        function __getMargin(level) {
            switch(level) {
            case 0:
                return DS.Style.settings.titleMarginL1
            case 1:
                return DS.Style.settings.titleMarginL2
            }
            return DS.Style.settings.titleMarginLOther
        }
    }

    background: Item {
        implicitHeight: DS.Style.settings.navigationHeight
        implicitWidth: DS.Style.settings.navigationWidth
        Rectangle {
            anchors.fill: parent
            visible: !control.checked
            color: control.D.ColorSelector.backgroundColor
            radius: DS.Style.control.radius
        }
        HighlightPanel {
            anchors.fill: parent
            visible: control.checked
        }
    }

    MouseArea {
        anchors.fill: parent
        Component.onCompleted: clicked.connect(control.clicked)
    }
}
