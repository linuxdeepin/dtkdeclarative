// SPDX-FileCopyrightText: 2022 - 2026 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

import QtQuick
import QtQuick.Window
import QtQuick.Templates as T
import org.deepin.dtk 1.0 as D
import org.deepin.dtk.style 1.0 as DS

T.Popup {
    id: control

    palette: D.DTK.palette
    focus: popupType === Popup.Window
    D.PopupHandle.enabled: popupType === Popup.Window

    property bool closeOnInactive: true
    readonly property bool active: parent && parent.Window.active

    // Sticky-max ratchet for Popup.Window: the popup's implicit size is by
    // default derived from background + contentItem, so keeping the largest
    // size during an open session is done via the background's implicit size
    // (windowBlurComponent) rather than overriding implicitWidth/Height here.
    // Content shrinking (e.g. search filtering) then no longer jumps the
    // popup window smaller. Reset on aboutToShow for each open session.
    property real _maxImplicitWidth: 0
    property real _maxImplicitHeight: 0

    onAboutToShow: {
        if (control.popupType === Popup.Window) {
            control._maxImplicitWidth = control.implicitContentWidth
                    + control.leftPadding + control.rightPadding
            control._maxImplicitHeight = control.implicitContentHeight
                    + control.topPadding + control.bottomPadding
        }
    }
    onImplicitContentWidthChanged: if (control.popupType === Popup.Window)
        control._maxImplicitWidth = Math.max(control._maxImplicitWidth,
            control.implicitContentWidth + control.leftPadding + control.rightPadding)
    onImplicitContentHeightChanged: if (control.popupType === Popup.Window)
        control._maxImplicitHeight = Math.max(control._maxImplicitHeight,
            control.implicitContentHeight + control.topPadding + control.bottomPadding)

    padding: DS.Style.popup.padding

    background: Loader {
        sourceComponent: control.popupType === Popup.Window
                         ? windowBlurComponent : floatingPanelComponent

        Component {
            id: windowBlurComponent
            D.StyledBehindWindowBlur {
                implicitWidth: control._maxImplicitWidth
                implicitHeight: control._maxImplicitHeight
            }
        }

        Component {
            id: floatingPanelComponent
            FloatingPanel {
                implicitHeight: DS.Style.popup.height
                implicitWidth: DS.Style.popup.width
                radius: DS.Style.popup.radius
            }
        }
    }

    onActiveChanged: {
        if (!active && closeOnInactive) {
            control.close()
        }
    }
}
