// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

import QtQuick 2.0
import QtQuick.Window 2.11
import org.deepin.dtk 1.0 as D

Item {
    // The width of the window's inner and outer borders do not change as the screen scales, and always remain 1px size
    property real borderWidth: 1 / Screen.devicePixelRatio
    property color color: D.DTK.themeType === D.ApplicationHelper.DarkType ? Qt.rgba(1, 1, 1, 0.15) : Qt.rgba(1, 1, 1, blendColorAlpha(0.4) - 0.05)
    property alias radius: rect.radius

    function blendColorAlpha(fallback) {
        var appearance = DS.applet("org.deepin.ds.dde-appearance")
        if (!appearance || appearance.opacity < 0)
            return fallback
        return appearance.opacity
    }

    Rectangle {
        id: rect

        anchors.fill: parent
        color: "transparent"

        border {
            width: borderWidth
            color: parent.color
        }
    }
}
