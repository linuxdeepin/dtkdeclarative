/*
 * Copyright (C) 2021 ~ 2022 UnionTech Technology Co., Ltd.
 *
 * Author:     yeshanshan <yeshanshan@uniontech.com>
 *
 * Maintainer: yeshanshan <yeshanshan@uniontech.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

import QtQuick 2.11
import QtQuick.Window 2.11
import QtQuick.Layouts 1.11
import org.deepin.dtk.impl 1.0 as D

RowLayout {
    id: control
    spacing: 0

    property bool fullScreenButtonVisible: true
    property bool embedMode: false
    signal maxOrWinded()

    property var __dwindow: Window.window.D.DWindow
    property bool __forceHind: !__dwindow.enabled || embedMode || Window.window.visibility === Window.FullScreen

    onMaxOrWinded: {
        if (!(__dwindow.motifFunctions & D.WindowManagerHelper.FUNC_MAXIMIZE)) {
            return
        }

        if (Window.window.visibility === Window.Maximized) {
            Window.window.visibility = Window.Windowed
        } else if (Window.window.visibility !== Window.FullScreen &&
                   maxOrWindedBtn.visible) {
            Window.window.visibility = Window.Maximized
        }
    }

    WindowButton {
        icon.name: "window_minimize"
        property bool hasWindowFlag/*: (Window.window.flags & Qt.WindowMinimizeButtonHint)*/
        Component.onCompleted: hasWindowFlag = (Window.window.flags & Qt.WindowMinimizeButtonHint)

        visible: hasWindowFlag &&  !__forceHind
        enabled: (__dwindow.motifFunctions & D.WindowManagerHelper.FUNC_MINIMIZE)

        onClicked: Window.window.visibility = Window.Minimized
    }

    WindowButton {
        icon.name: "window_quit_full"
        visible: !(!control.fullScreenButtonVisible ||
                    !__dwindow.enabled ||
                    Window.window.visibility !== Window.FullScreen)

        onClicked: {
            if (Window.window.visibility === Window.FullScreen) {
                Window.window.visibility = Window.Windowed
            } else {
                Window.window.visibility = Window.FullScreen
            }
        }
    }

    WindowButton {
        id: maxOrWindedBtn
        property bool isMaximized: Window.window.visibility === Window.Maximized
        icon.name: isMaximized ? "window_restore" : "window_maximize"

        onClicked: maxOrWinded()

        property bool hasWindowFlag/*: (Window.window.flags & Qt.WindowMaximizeButtonHint)*/
        Component.onCompleted: hasWindowFlag = (Window.window.flags & Qt.WindowMaximizeButtonHint)

        readonly property size maxSize: Qt.size(Window.window.maximumWidth, Window.window.maximumHeight)
        readonly property size minSize: Qt.size(Window.window.minimumWidth, Window.window.minimumHeight)
        visible: (hasWindowFlag && !__forceHind &&
                    (__dwindow.motifFunctions & D.WindowManagerHelper.FUNC_RESIZE) &&
                    maxSize != minSize)

        enabled: ((__dwindow.motifFunctions & D.WindowManagerHelper.FUNC_MAXIMIZE) &&
                    (__dwindow.motifFunctions & D.WindowManagerHelper.FUNC_RESIZE))
    }

    WindowButton {
        icon.name: "window_close"
        property bool hasWindowFlag/*: (Window.window.flags & Qt.WindowCloseButtonHint)*/
        Component.onCompleted: hasWindowFlag = (Window.window.flags & Qt.WindowCloseButtonHint)

        visible: hasWindowFlag && __dwindow.enabled
        enabled: __dwindow.motifFunctions & D.WindowManagerHelper.FUNC_CLOSE
        onClicked: Window.window.close()
    }
}
