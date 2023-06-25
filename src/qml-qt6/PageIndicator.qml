// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

import QtQuick 2.11
import QtQuick.Templates 2.4 as T
import "PixelMetric.js" as PM

T.PageIndicator {
    id: control

    implicitWidth: Math.max(background ? background.implicitWidth : 0,
                            contentItem.implicitWidth + leftPadding + rightPadding)
    implicitHeight: Math.max(background ? background.implicitHeight : 0,
                             contentItem.implicitHeight + topPadding + bottomPadding)

    padding: PM.ControlPadding
    spacing: PM.ControlPadding

    delegate: Rectangle {
        implicitWidth: PM.PageIndicator_ImplicitWidth
        implicitHeight: PM.PageIndicator_ImplicitHeight

        radius: width / 2
        color: control.enabled ? control.palette.highlight : "gray"

        opacity: index === currentIndex ? 0.95 : pressed ? 0.7 : 0.45
        Behavior on opacity { OpacityAnimator { duration: 100 } }
    }

    contentItem: Row {
        spacing: control.spacing

        Repeater {
            model: control.count
            delegate: control.delegate
        }
    }
}
