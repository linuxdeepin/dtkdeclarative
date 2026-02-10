// SPDX-FileCopyrightText: 2021 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

import QtQuick
import QtQuick.Layouts
import QtQuick.Window
import org.deepin.dtk 1.0 as D
import org.deepin.dtk.style 1.0 as DS

Window {
    id: control

    maximumWidth: Screen.desktopAvailableWidth
    maximumHeight: Screen.desktopAvailableHeight
    minimumWidth: DS.Style.dialogWindow.width
    minimumHeight: DS.Style.dialogWindow.height
    D.DWindow.enabled: true
    D.DWindow.enableSystemResize: false
    D.DWindow.motifFunctions: D.WindowManagerHelper.FUNC_ALL & ~D.WindowManagerHelper.FUNC_RESIZE
    D.DWindow.wmWindowTypes: D.WindowManagerHelper.DialogType
    D.DWindow.enableBlurWindow: true
    flags: Qt.Dialog | Qt.WindowCloseButtonHint | Qt.MSWindowsFixedSizeDialogHint
    D.ColorSelector.family: D.Palette.CrystalColor
    color: active ? D.DTK.palette.window : D.DTK.inactivePalette.window
    height: content.height
    width: content.width

    property alias header: titleBar.sourceComponent
    property string icon
    default property alias content: contentLoader.children
    property alias palette : content.palette
    property real leftPadding: DS.Style.dialogWindow.contentHMargin
    property real rightPadding: DS.Style.dialogWindow.contentHMargin

    property var transientParentWindow: null
    transientParent: transientParentWindow

    Item {
        id: content
        palette: control.active ? D.DTK.palette : D.DTK.inactivePalette
        implicitHeight: childrenRect.height
        ColumnLayout {
            id: layout
            spacing: 0

            Loader {
                id: titleBar
                z: D.DTK.TopOrder
                sourceComponent: DialogTitleBar {
                    enableInWindowBlendBlur: true
                    icon.name: control.icon
                }
            }

            Item {
                id: contentLoader
                Layout.fillWidth: true
                Layout.preferredHeight: childrenRect.height
                Layout.leftMargin: control.leftPadding
                Layout.rightMargin: control.rightPadding
            }
        }
    }

    Component.onCompleted: {
        if (control.modality === Qt.WindowModal && !transientParentWindow)
            transientParentWindow = Qt.application.activeWindow
    }

    onVisibleChanged: {
        if (!control.visible)
            return
        if (control.modality !== Qt.WindowModal)
            return
        if (!transientParentWindow || transientParentWindow === control) {
            var candidate = Qt.application.activeWindow
            if (candidate && candidate !== control)
                transientParentWindow = candidate
        }
        Qt.callLater(function () {
            control.raise()
            control.requestActivate()
        })
    }

    onClosing: function(close) {
        // close can't reset sub control's hovered state. pms Bug:168405
        // if we need to close, we can add closing handler to set `close.acceped = true`
        close.accepted = false
        hide()
    }
}
