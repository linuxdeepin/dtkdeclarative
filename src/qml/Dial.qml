// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

import QtQuick 2.11
import QtQuick.Controls.impl 2.4
import QtQuick.Templates 2.4 as T
import "PixelMetric.js" as PM

T.Dial {
    id: control

    implicitWidth: Math.max(background ? background.implicitWidth : 0,
                            contentItem ? contentItem.implicitWidth + leftPadding + rightPadding : 0)
    implicitHeight: Math.max(background ? background.implicitHeight : 0,
                             contentItem ? contentItem.implicitHeight + topPadding + bottomPadding : 0)

    background: DialImpl {
        implicitWidth: PM.Dial_miniSize
        implicitHeight: PM.Dial_miniSize
        color: control.palette.highlight
        progress: control.position
    }

    handle: Rectangle {
        id: handleItem
        anchors.centerIn: control.background
        width: 2 * PM.ControlRadius
        height: 2 * PM.ControlRadius
        color: control.palette.highlight
        radius: PM.ControlRadius
        antialiasing: true
        transform: [
            Translate {
                y: -Math.min(control.background.width, control.background.height) * 0.4 + handleItem.height / 2
            },
            Rotation {
                angle: control.angle
                origin.x: handleItem.width / 2
                origin.y: handleItem.height / 2
            }
        ]
    }
}
