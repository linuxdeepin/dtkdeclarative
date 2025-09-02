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

        // Unified scroll operation function
        function performScroll() {
            direction === ArrowListViewButton.UpButton ? view.decrementCurrentIndex() 
                                                       : view.incrementCurrentIndex()
        }

        // Auto-scroll control properties using state machine approach
        property bool shouldAutoScroll: hovered && enabled
        property bool delayCompleted: false

        // Timer for initial delay before starting hover scroll
        Timer {
            id: initialDelayTimer
            interval: 300
            repeat: false
            running: shouldAutoScroll && !delayCompleted
            onTriggered: delayCompleted = true
        }

        // Timer for continuous hover scrolling
        Timer {
            id: hoverScrollTimer
            interval: 100
            repeat: true
            running: shouldAutoScroll && delayCompleted
            onTriggered: performScroll()
        }
                
        // Reset state when auto-scroll should stop
        onShouldAutoScrollChanged: {
            if (!shouldAutoScroll) {
                delayCompleted = false
            }
        }
    }
}
