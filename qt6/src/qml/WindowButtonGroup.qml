// SPDX-FileCopyrightText: 2021 - 2026 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

import QtQuick
import QtQuick.Window
import QtQuick.Layouts
import org.deepin.dtk 1.0 as D
import org.deepin.dtk.style 1.0 as DS

RowLayout {
    id: control
    spacing: 0

    property D.Palette textColor: DS.Style.button.text
    property bool fullScreenButtonVisible: true
    property bool embedMode: false
    signal maxOrWinded()

    property var __dwindow: Window.window.D.DWindow
    property bool __forceHind: !__dwindow.enabled || embedMode || Window.window.visibility === Window.FullScreen
    
    // 缓存 motifFunctions 位运算结果，避免重复计算
    readonly property bool __canMinimize: __dwindow.motifFunctions & D.WindowManagerHelper.FUNC_MINIMIZE
    readonly property bool __canMaximize: __dwindow.motifFunctions & D.WindowManagerHelper.FUNC_MAXIMIZE
    readonly property bool __canResize: __dwindow.motifFunctions & D.WindowManagerHelper.FUNC_RESIZE
    readonly property bool __canClose: __dwindow.motifFunctions & D.WindowManagerHelper.FUNC_CLOSE

    onMaxOrWinded: {
        if (!__canMaximize) {
            return
        }

        if (Window.window.visibility === Window.Maximized) {
            __dwindow.showNormal()
        } else if (Window.window.visibility !== Window.FullScreen &&
                   maxOrWindedBtn.visible) {
            __dwindow.showMaximized()
        }
    }

    WindowButton {
        objectName: "minimizeBtn"
        property bool hasWindowFlag/*: (Window.window.flags & Qt.WindowMinimizeButtonHint)*/
        Component.onCompleted: hasWindowFlag = (Window.window.flags & Qt.WindowMinimizeButtonHint)
        
        icon.name: "window_minimize"
        textColor: control.textColor
        
        visible: hasWindowFlag && !__forceHind
        enabled: __canMinimize

        onClicked: {
            __dwindow.showMinimized()
        }
    }

    WindowButton {
        objectName: "quitFullBtn"
        
        icon.name: "window_quit_full"
        textColor: control.textColor
        
        visible: !(!control.fullScreenButtonVisible ||
                   !__dwindow.enabled ||
                   Window.window.visibility !== Window.FullScreen)

        onClicked: {
            if (Window.window.visibility === Window.FullScreen) {
                __dwindow.showNormal()
            } else {
                __dwindow.showFullScreen()
            }
        }
    }

    WindowButton {
        id: maxOrWindedBtn; objectName: "maxOrWindedBtn"
        property bool hasWindowFlag/*: (Window.window.flags & Qt.WindowMaximizeButtonHint)*/
        Component.onCompleted: hasWindowFlag = (Window.window.flags & Qt.WindowMaximizeButtonHint)

        // 使用简单值比较代替对象创建，避免性能开销
        readonly property bool __sizeResizable: (Window.window.maximumWidth !== Window.window.minimumWidth ||
                                                  Window.window.maximumHeight !== Window.window.minimumHeight)
        
        property bool isMaximized: Window.window.visibility === Window.Maximized
        icon.name: isMaximized ? "window_restore" : "window_maximize"
        textColor: control.textColor
        onClicked: maxOrWinded()
        
        visible: (hasWindowFlag && !__forceHind && __canResize && __sizeResizable)
        enabled: (__canMaximize && __canResize)
    }

    WindowButton {
        objectName: "closeBtn"
        property bool hasWindowFlag/*: (Window.window.flags & Qt.WindowCloseButtonHint)*/
        Component.onCompleted: hasWindowFlag = (Window.window.flags & Qt.WindowCloseButtonHint)
        
        icon.name: "window_close"
        textColor: control.textColor
        
        visible: hasWindowFlag && __dwindow.enabled
        enabled: __canClose
        
        onClicked: Window.window.close()
    }
}
