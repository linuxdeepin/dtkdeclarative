// SPDX-FileCopyrightText: 2025 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

import QtQuick 2.0
import QtQuick.Layouts 1.11
import org.deepin.dtk 1.0

Flow {
    id: control
    Row {
        spacing: 20

        GroupBox {
            title: "GroupBox"
            ColumnLayout {
                anchors.fill: parent
                CheckBox { text: "E-mail" }
                CheckBox { text: "Super Fancy Calendar" }
                CheckBox { text: "Contacts" }
            }
        }

        GroupBox {
            title: "GroupBox 2"
            Label {
                text: "GroupBox Content"
            }
        }
    }
}
