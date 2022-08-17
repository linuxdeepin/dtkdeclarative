// SPDX-FileCopyrightText: 2021 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

import QtQuick 2.11

Action {
    id: control; objectName: "_d_about_action"
    text: qsTr("About")
    property Component aboutDialog
    property QtObject object
    onTriggered: {
        if (aboutDialog) {
            if (!object) {
                object = aboutDialog.createObject(parent)
            }
            object.show()
        }
    }
}
