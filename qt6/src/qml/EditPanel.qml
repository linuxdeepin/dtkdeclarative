// SPDX-FileCopyrightText: 2021 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

import QtQuick
import org.deepin.dtk 1.0 as D
import org.deepin.dtk.style 1.0 as DS


Rectangle {
    id: panel
    property Item control
    property D.Palette backgroundColor: DS.Style.edit.background
    property D.Palette alertBackgroundColor: DS.Style.edit.alertBackground
    property alias showBorder: _border.active
    property bool showAlert: false
    property string alertText: ""
    property int alertDuration: 0

    radius: DS.Style.control.radius
    color: showAlert ? D.ColorSelector.alertBackgroundColor
                     : D.ColorSelector.backgroundColor

    Loader {
        id: _border
        anchors.fill: parent
        active: control.activeFocus
        sourceComponent: FocusBoxBorder {
            color: control.palette.highlight
            radius: panel.radius
        }
    }

    Loader {
        active: showAlert && alertText.length !== 0
        sourceComponent: AlertToolTip {
            target: control
            timeout: alertDuration
            visible: showAlert
            text: alertText
        }
    }
}
