// SPDX-FileCopyrightText: 2021 - 2026 UnionTech Software Technology Co., Ltd.
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
    color: D.DWindow.enableBlurWindow ? "transparent" : (active ? D.DTK.palette.window : D.DTK.inactivePalette.window)
    height: content.height
    width: content.width

    property alias header: titleBar.sourceComponent
    property string icon
    default property alias content: contentLoader.children
    property alias palette : content.palette
    property real leftPadding: DS.Style.dialogWindow.contentHMargin
    property real rightPadding: DS.Style.dialogWindow.contentHMargin
    
    D.StyledBehindWindowBlur {
        control: control
        anchors.fill: parent
        blendColor: {
            if (valid) {
                return DS.Style.control.selectColor(undefined,
                    Qt.rgba(235 / 255.0, 235 / 255.0, 235 / 255.0, 0.6),
                    Qt.rgba(10 / 255.0, 10 / 255.0, 10 / 255.0, 85 / 255.0))
            }
            return DS.Style.control.selectColor(undefined,
                DS.Style.behindWindowBlur.lightNoBlurColor,
                DS.Style.behindWindowBlur.darkNoBlurColor)
        }
    }

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
                    enableInWindowBlendBlur: false
                    icon.name: control.icon
                    title: control.title
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
    
    onClosing: function(close) {
        // close can't reset sub control's hovered state. pms Bug:168405
        // if we need to close, we can add closing handler to set `close.acceped = true`
        close.accepted = false
        hide()
    }
}
