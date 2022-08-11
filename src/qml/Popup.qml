// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

import QtQuick 2.11
import QtQuick.Templates 2.4 as T
import org.deepin.dtk.impl 1.0 as D
import org.deepin.dtk.style 1.0 as DS

T.Popup {
    id: control

    implicitWidth: DS.Style.control.implicitWidth(control)
    implicitHeight: DS.Style.control.implicitHeight(control)

    padding: DS.Style.popup.padding

    background: Loader {
        active: !control.D.PopupHandle.window
        sourceComponent: FloatingPanel {
            implicitHeight: DS.Style.popup.height
            implicitWidth: DS.Style.popup.width
            radius: DS.Style.popup.radius
        }
    }
}
