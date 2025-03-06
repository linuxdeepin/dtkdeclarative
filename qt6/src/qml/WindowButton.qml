// SPDX-FileCopyrightText: 2021 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

import QtQuick
import org.deepin.dtk 1.0 as D
import org.deepin.dtk.style 1.0 as DS

D.IconButton {
    id: control

    property int topRightRadius: (Window.window.visibility !== Window.Maximized &&
                                 Window.window.visibility !== Window.FullScreen &&
                                 isOnRightEdgeOfWindow) ? D.DTK.platformTheme.windowRadius : 0
    readonly property bool isOnRightEdgeOfWindow: __itemGlobalPoint.x + control.width >= Window.window.width

    readonly property var __itemGlobalPoint: {
        var a = control
        var x = 0, y = 0
        while(a.parent) {
            x += a.x
            y += a.y
            a = a.parent
        }
        return Qt.point(x, y)
    }

    topPadding: 0
    bottomPadding: 0
    leftPadding: 0
    rightPadding: 0
    icon {
        width: DS.Style.windowButton.width
        height: DS.Style.windowButton.height
    }
    background: D.BoxPanel {
        implicitWidth: DS.Style.windowButton.width
        implicitHeight: DS.Style.windowButton.height
        insideBorderColor: null
        outsideBorderColor: null
        color1: DS.Style.windowButton.background
        color2: color1
        radius: 0

        Loader {
            anchors.fill: parent
            active: control.visualFocus
            sourceComponent: Rectangle {
                topRightRadius: control.topRightRadius
                color: "transparent"
                border {
                    width: DS.Style.control.focusBorderWidth
                    color: control.palette.highlight
                }
            }
        }
    }
}
