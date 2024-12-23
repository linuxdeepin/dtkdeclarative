// SPDX-FileCopyrightText: 2021 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

import QtQuick
import QtQuick.Layouts
import org.deepin.dtk.settings 1.0 as Settings
import org.deepin.dtk 1.0

Settings.OptionDelegate {
    id: control

    LineEdit {
        id: impl
        Layout.alignment: Qt.AlignRight
        Layout.fillWidth: true

        text: Settings.SettingsOption.value || ""

        onEditingFinished: {
            Settings.SettingsOption.value = text
        }
    }
}
