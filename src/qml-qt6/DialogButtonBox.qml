// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

import QtQuick 2.11
import QtQuick.Templates 2.4 as T
import "PixelMetric.js" as PM

T.DialogButtonBox {
    id: control

    implicitWidth: Math.max(background ? background.implicitWidth : 0,
                            contentItem.implicitWidth + leftPadding + rightPadding)
    implicitHeight: Math.max(background ? background.implicitHeight : 0,
                             contentItem.implicitHeight + topPadding + bottomPadding)

    padding: PM.ControlPadding
    spacing: PM.ControlSpacing
    alignment: Qt.AlignRight

    delegate: Button { }

    contentItem: ListView {
        implicitWidth: contentWidth
        implicitHeight: PM.Button_MiniSize

        model: control.contentModel
        spacing: control.spacing
        orientation: ListView.Horizontal
        boundsBehavior: Flickable.StopAtBounds
        snapMode: ListView.SnapToItem
    }
}