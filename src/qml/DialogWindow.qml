// SPDX-FileCopyrightText: 2021 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

import QtQuick 2.11
import QtQuick.Layouts 1.11
import QtQuick.Window 2.11
import org.deepin.dtk.impl 1.0 as D
import org.deepin.dtk.style 1.0 as DS

Window {
    id: control

    width: implicitWidth
    height: implicitHeight
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

    readonly property int maxContentHeight: height - titleBar.height
    readonly property int implicitWidth: layout.implicitWidth
    readonly property int implicitHeight: titleBar.height + contentLoader.childrenRect.height
    property alias header: titleBar.sourceComponent
    property string icon
    default property alias content: contentLoader.children

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
            Layout.preferredHeight: control.height - titleBar.height
            Layout.fillWidth: true
            Layout.leftMargin: DS.Style.dialogWindow.contentHMargin
            Layout.rightMargin: DS.Style.dialogWindow.contentHMargin
        }
    }
}
