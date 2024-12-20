// SPDX-FileCopyrightText: 2021 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

import QtQuick
import QtQuick.Layouts
import org.deepin.dtk.settings 1.0
import org.deepin.dtk 1.0

RowLayout {
    id: control
    width: parent.width - parent.leftPadding - parent.rightPadding

    property bool leftVisible: true
    Label {
        id: label
        visible: leftVisible
        text: SettingsOption.name
        verticalAlignment: Text.AlignVCenter
    }
    Item {
        visible: leftVisible
        width: 20
    }
}
