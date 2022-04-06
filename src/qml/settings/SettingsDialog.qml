/*
 * Copyright (C) 2021 ~ 2022 Uniontech Technology Co., Ltd.
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
import org.deepin.dtk.impl 1.0 as D
import org.deepin.dtk.style 1.0 as DS
import org.deepin.dtk.settings 1.0 as Settings
import ".."

DialogWindow {
    id: control

    property list<Settings.SettingsGroup> groups
    property D.Config config
    property Settings.SettingsContainer container : Settings.SettingsContainer {
        id: settingsContainer
        config: control.config
        navigationTitle: Settings.NavigationTitle {
            width: navigationView.width - navigationView.leftMargin - navigationView.rightMargin
            checked: navigationView.currentIndex === Settings.SettingsGroup.index
            onClicked: navigationView.currentIndex = Settings.SettingsGroup.index
        }
        contentTitle: Settings.ContentTitle {}
        contentBackground: Settings.ContentBackground {}
        groups: control.groups
    }
    property alias navigationView: navigationView
    property alias contentView: contentView

    ScrollView {
        id: navigationBg
        width: DS.Style.settings.navigation.width
        background: Rectangle {
            anchors.fill: parent
            color: palette.base
        }

        ListView {
            id: navigationView
            model: container.navigationModel
            leftMargin: DS.Style.settings.navigation.margin
            rightMargin: DS.Style.settings.navigation.margin
            currentIndex: 0
            onCurrentIndexChanged: {
                contentView.currentIndex = currentIndex
            }
        }
    }

    ScrollView {
        id: contentBg
        width: control.width - navigationBg.width
        anchors {
            right: parent.right
            top: control.top
        }
        padding: DS.Style.settings.content.margin
        background: Rectangle {
            anchors.fill: parent
            color: palette.base
        }

        ListView {
            id: contentView
            model: container.contentModel
            highlightRangeMode: ListView.StrictlyEnforceRange
            currentIndex: 0
            onCurrentIndexChanged: {
                navigationView.currentIndex = currentIndex
            }
        }
    }
}
