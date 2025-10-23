// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

import QtQuick
import QtQuick.Templates as T
import org.deepin.dtk 1.0 as D
import org.deepin.dtk.style 1.0 as DS

T.ScrollBar {
    id: control

    implicitWidth: DS.Style.control.implicitWidth(control)
    implicitHeight: DS.Style.control.implicitHeight(control)

    padding: DS.Style.scrollBar.padding
    visible: control.policy !== T.ScrollBar.AlwaysOff && control.size < 1.0
    policy: D.DTK.platformTheme.scrollBarPolicy

    state: "hide"
    property int __currentState: 0
    function __calculateStateConditions() {
        var moving = control.active && !control.pressed && !control.hovered
        var shouldNormal = (control.policy === T.ScrollBar.AlwaysOn && !control.hovered && !control.pressed) || 
                       (moving && control.size < 1.0)
        
        var shouldHover = ((control.hovered && (control.contentItem.opacity > 0 && control.contentItem.opacity <= 1))  && control.active && !control.pressed && control.size < 1.0)
        
        var shouldActive = (control.pressed && control.size < 1.0)
        
        var shouldHide = !shouldNormal && !shouldHover && !shouldActive

        __currentState = shouldHide ? 0 : shouldNormal ? 1 : shouldHover ? 2 : 3
    }
    
    onHoveredChanged: {
       Qt.callLater(() => { __calculateStateConditions() })
    }
    onPressedChanged: {
       Qt.callLater(() => { __calculateStateConditions() })
    }
    onActiveChanged: {
       Qt.callLater(() => { __calculateStateConditions() })
    }
    onSizeChanged: {
       Qt.callLater(() => { __calculateStateConditions() })
    }
    onPolicyChanged: {
       Qt.callLater(() => { __calculateStateConditions() })
    }

    states: [
        State {
            name: "hide"
            when: __currentState === 0
            PropertyChanges {
                target: control.contentItem
                implicitWidth: DS.Style.scrollBar.width
                opacity: DS.Style.scrollBar.hideOpacity
            }
        },
        State {
            name: "normal"
            when: __currentState === 1
            PropertyChanges {
                target: control.contentItem
                implicitWidth: DS.Style.scrollBar.width
            }
        },
        State {
            name: "hover"
            when: __currentState === 2
            PropertyChanges {
                target: control.contentItem
                implicitWidth: DS.Style.scrollBar.activeWidth
            }
        },
        State {
            name: "active"
            when: __currentState === 3
            PropertyChanges {
                target: control.contentItem
                implicitWidth: DS.Style.scrollBar.activeWidth
            }
        }
    ]

    transitions: Transition {
        to: "hide"
        SequentialAnimation {
            NumberAnimation { target: control.contentItem; duration: DS.Style.scrollBar.hideWidthDuration; property: "implicitWidth";}
            PauseAnimation { duration: DS.Style.scrollBar.hidePauseDuration }
            NumberAnimation { target: control.contentItem; duration: DS.Style.scrollBar.hideDuration; property: "opacity";}
        }
    }

    contentItem: Item {
        id: content
        property D.Palette backgroundColor: DS.Style.scrollBar.background
        property D.Palette insideBorderColor: DS.Style.scrollBar.insideBorder
        property D.Palette outsideBorderColor: DS.Style.scrollBar.outsideBorder

        implicitWidth: DS.Style.scrollBar.width
        implicitHeight: implicitWidth

        Rectangle {
            id: backgroundRect
            anchors.fill: parent
            radius: control.orientation == Qt.Horizontal ? control.height / 2 : control.width / 2
            color: content.D.ColorSelector.backgroundColor
        }

        InsideBoxBorder {
            anchors.fill: backgroundRect
            radius: backgroundRect.radius
            color: content.D.ColorSelector.insideBorderColor
            borderWidth: DS.Style.control.borderWidth
        }

        OutsideBoxBorder {
            anchors.fill: backgroundRect
            radius: backgroundRect.radius
            color: content.D.ColorSelector.outsideBorderColor
            borderWidth: DS.Style.control.borderWidth
        }
    }
}
