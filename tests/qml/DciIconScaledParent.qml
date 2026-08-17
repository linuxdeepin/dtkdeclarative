// SPDX-FileCopyrightText: 2026 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

import QtQuick 2.11
import org.deepin.dtk 1.0 as D

Item {
    id: root
    width: 100; height: 100

    property alias scaledParent: scaledParent
    property alias icon: icon

    Item {
        id: scaledParent
        x: 0.3
        y: 0.3
        width: 100; height: 100
        scale: 0.1

        D.DciIcon {
            id: icon
            objectName: "scaledDciIcon"
            width: 100; height: 100
            sourceSize: Qt.size(17, 17)
            name: "switch_button"
        }
    }
}
