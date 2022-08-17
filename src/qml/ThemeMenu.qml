// SPDX-FileCopyrightText: 2021 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

import QtQuick 2.11
import org.deepin.dtk.impl 1.0 as D

Menu {
    id: control

    title: qsTr("Theme")
    ActionGroup {
        id: themeEG
        exclusive: true
        onTriggered: {
            D.ApplicationHelper.setPaletteType(action.themeType)
        }
    }

    Action {
        text: qsTr("Light Theme")
        readonly property int themeType: D.ApplicationHelper.LightType
        checked: D.DTK.themeType === themeType
        ActionGroup.group: themeEG
    }
    Action {
        text: qsTr("Dark Theme")
        readonly property int themeType: D.ApplicationHelper.DarkType
        checked: D.DTK.themeType === themeType
        ActionGroup.group: themeEG
    }
    Action {
        text: qsTr("System Theme")
        readonly property int themeType: D.ApplicationHelper.UnknownType
        ActionGroup.group: themeEG
    }
}
