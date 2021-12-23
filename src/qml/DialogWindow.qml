/*
 * Copyright (C) 2021 UnionTech Technology Co., Ltd.
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
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.11
import QtQuick.Window 2.11
import org.deepin.dtk.impl 1.0 as D
import "PixelMetric.js" as PM

Window {
    id: control

    height: Math.min(maxDialogWindowHeight, titleBar.height + contentLoader.childrenRect.height)
    width: 380
    maximumWidth: width
    maximumHeight: height
    minimumWidth: maximumWidth
    minimumHeight: maximumHeight
    D.Window.enabled: true
    D.Window.enableSystemResize: false
    D.Window.enableBlurWindow: true
    flags: Qt.Dialog | Qt.WindowCloseButtonHint | Qt.MSWindowsFixedSizeDialogHint

    readonly property int maxDialogWindowHeight: PM.DialogWindow_MaximumHeight
    readonly property int maxContentHeight: maxDialogWindowHeight - titleBar.height
    property alias titleBar: titleBar.sourceComponent

    default property alias content: contentLoader.children

    ColumnLayout {
        id: layout
        spacing: 0
        anchors.fill: parent

        Loader {
            id: titleBar
            z: D.DTK.TopOrder
            sourceComponent: DialogTitleBar {}
        }

        Item {
            id: contentLoader
            Layout.fillHeight: true
            Layout.fillWidth: true
        }
    }
}
