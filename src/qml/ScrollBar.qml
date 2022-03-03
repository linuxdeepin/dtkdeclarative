/*
 * Copyright (C) 2022 UnionTech Technology Co., Ltd.
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
import QtQuick.Templates 2.4 as T
import org.deepin.dtk.impl 1.0 as D
import org.deepin.dtk.style 1.0 as DS

T.ScrollBar {
    id: control

    implicitWidth: DS.Style.control.implicitWidth(control)
    implicitHeight: DS.Style.control.implicitHeight(control)

    padding: DS.Style.scrollBar.padding
    visible: control.policy !== T.ScrollBar.AlwaysOff

    state: "hide"
    states: [
        State {
            name: "hide"
            when: control.policy === T.ScrollBar.AlwaysOff || !control.active || control.size >= 1.0
            PropertyChanges {
                target: control.contentItem
                implicitWidth: DS.Style.scrollBar.width
                opacity: DS.Style.scrollBar.hideOpacity
            }
        },
        State {
            name: "normal"
            property bool moving: control.active && !control.pressed && !control.hovered
            when: control.policy === T.ScrollBar.AlwaysOn || ( moving && control.size < 1.0)
            PropertyChanges {
                target: control.contentItem
                implicitWidth: DS.Style.scrollBar.width
                opacity: DS.Style.scrollBar.normalOpacity
            }
        },
        State {
            name: "hover"
            when: control.policy === T.ScrollBar.AlwaysOn || ( control.hovered && !control.pressed && control.size < 1.0)
            PropertyChanges {
                target: control.contentItem
                implicitWidth: DS.Style.scrollBar.activeWidth
                opacity: DS.Style.scrollBar.hoverOpacity
            }
        },
        State {
            name: "active"
            when: control.policy === T.ScrollBar.AlwaysOn || (control.pressed && control.size < 1.0)
            PropertyChanges {
                target: control.contentItem
                implicitWidth: DS.Style.scrollBar.activeWidth
                opacity: DS.Style.scrollBar.activeOpacity
            }
        }
    ]

    transitions: Transition {
        to: "hide"
        SequentialAnimation {
            PauseAnimation { duration: DS.Style.scrollBar.hidePauseDuration }
            NumberAnimation { target: control.contentItem; duration: DS.Style.scrollBar.hideDuration; property: "opacity"; to:DS.Style.scrollBar.hideOpacity }
        }
    }

    contentItem: Item {
        implicitWidth: DS.Style.scrollBar.width
        implicitHeight: implicitWidth

        BoxShadow {
            anchors.fill: backgroundRect
            shadowColor: DS.Style.scrollBar.outerShadowColor
            spread: 1
            cornerRadius: backgroundRect.radius
            hollow: true
        }

        Rectangle {
            id: backgroundRect
            anchors.fill: parent
            radius: control.width / 2
            color: DS.Style.selectColor(control.palette.window, DS.Style.scrollBar.lightBackground,
                                        DS.Style.scrollBar.darkBackground)
            border.width: DS.Style.control.borderWidth
        }

        BoxInsetShadow {
            anchors.fill: backgroundRect
            shadowColor: DS.Style.scrollBar.innerShadowColor
            spread: 1
            cornerRadius: backgroundRect.radius
        }
    }
}
