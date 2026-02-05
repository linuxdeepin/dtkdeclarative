// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

import QtQuick
import org.deepin.dtk.style 1.0 as DS
import org.deepin.dtk 1.0

Loader {
    enum Direction {
        UpButton = 0,
        DownButton = 1
    }

    required property Item view
    property int direction
    property int stepSize: DS.Style.arrowListView.itemHeight
    active: view.interactive

    sourceComponent: ActionButton {
        palette.windowText: undefined
        enabled: direction === ArrowListViewButton.UpButton ? !view.atYBeginning : !view.atYEnd
        width: DS.Style.arrowListView.stepButtonSize.width
        height: DS.Style.arrowListView.stepButtonSize.height
        icon.name: direction === ArrowListViewButton.UpButton ? DS.Style.arrowListView.upButtonIconName
                                                              : DS.Style.arrowListView.downButtonIconName
        icon.width: DS.Style.arrowListView.stepButtonIconSize.width
        icon.height: DS.Style.arrowListView.stepButtonIconSize.height

        function performScroll() {
            if (!view)
                return
            var maxY = Math.max(0, view.contentHeight - view.height)
            if (maxY <= 0)
                return
            var step = Math.max(1, stepSize)
            var nextY = direction === ArrowListViewButton.UpButton ? (view.contentY - step)
                                                                   : (view.contentY + step)
            view.contentY = Math.max(0, Math.min(maxY, nextY))
        }

        property bool shouldAutoScroll: hovered && enabled
        property bool delayCompleted: false

        Timer {
            id: initialDelayTimer
            interval: 300
            repeat: false
            running: shouldAutoScroll && !delayCompleted
            onTriggered: delayCompleted = true
        }

        Timer {
            id: hoverScrollTimer
            interval: 100
            repeat: true
            running: shouldAutoScroll && delayCompleted
            onTriggered: performScroll()
        }
                
        onShouldAutoScrollChanged: {
            if (!shouldAutoScroll)
                delayCompleted = false
        }

        onClicked: performScroll()
    }
}
