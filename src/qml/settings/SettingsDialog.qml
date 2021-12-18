/*
 * Copyright (C) 2021 Uniontech Technology Co., Ltd.
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
import QtQuick.Layouts 1.11
import org.deepin.dtk 1.0
import org.deepin.dtk.settings 1.0 as Settings

DialogWindow {
    id: control

    property list<Settings.SettingsGroup> groups
    property Config config

    property Settings.SettingsContainer container : Settings.SettingsContainer {
        id: settingsContainer
        config: control.config
        navigationTitle: Settings.NavigationTitle {}
        contentTitle: Settings.ContentTitle {}
        contentBackground: Settings.ContentBackground {}

        groups: control.groups
    }

    ScrollView {
        id: navigationBg
        width: 190
        contentWidth: width
        background: Rectangle {
            anchors.fill: parent
            color: palette.base
        }

        ListView {
            id: navigationView
            model: container.navigationModel
            currentIndex: 0
            onCurrentIndexChanged: {
                contentView.currentIndex = currentIndex
            }
        }
    }

    ScrollView {
        id: contentBg
        width: control.width - navigationBg.width
        contentWidth: width
        anchors {
            right: parent.right
            top: control.top
        }
        background: Rectangle {
            anchors.fill: parent
            color: palette.base
        }

        ListView {
            id: contentView; objectName: "contentView"
            model: container.contentModel
            highlightRangeMode: ListView.StrictlyEnforceRange
            currentIndex: 0
            onCurrentIndexChanged: {
                navigationView.currentIndex = currentIndex
            }
        }
    }
}
