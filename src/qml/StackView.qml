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
import org.deepin.dtk.style 1.0 as DS

T.StackView {
    id: control

    popEnter: Transition {
        // slide_in_left
        NumberAnimation { property: "x"; from: (control.mirrored ? -0.5 : 0.5) *  -control.width; to: 0; duration: DS.Style.stackView.animationDuration; easing.type: DS.Style.stackView.animationEasingType }
        NumberAnimation { property: "opacity"; from: 0.0; to: 1.0; duration: DS.Style.stackView.animationDuration; easing.type: DS.Style.stackView.animationEasingType }
    }

    popExit: Transition {
        // slide_out_right
        NumberAnimation { property: "x"; from: 0; to: (control.mirrored ? -0.5 : 0.5) * control.width; duration: DS.Style.stackView.animationDuration; easing.type: Easing.OutCubic }
        NumberAnimation { property: "opacity"; from: 1.0; to: 0.0; duration: DS.Style.stackView.animationDuration; easing.type: DS.Style.stackView.animationEasingType }
    }

    pushEnter: Transition {
        // slide_in_right
        NumberAnimation { property: "x"; from: (control.mirrored ? -0.5 : 0.5) * control.width; to: 0; duration: DS.Style.stackView.animationDuration; easing.type: Easing.OutCubic }
        NumberAnimation { property: "opacity"; from: 0.0; to: 1.0; duration: DS.Style.stackView.animationDuration; easing.type: DS.Style.stackView.animationEasingType }
    }

    pushExit: Transition {
        // slide_out_left
        NumberAnimation { property: "x"; from: 0; to: (control.mirrored ? -0.5 : 0.5) * -control.width; duration: DS.Style.stackView.animationDuration; easing.type: Easing.OutCubic }
        NumberAnimation { property: "opacity"; from: 1.0; to: 0.0; duration: DS.Style.stackView.animationDuration; easing.type: DS.Style.stackView.animationEasingType }
    }

    replaceEnter: Transition {
        // slide_in_right
        NumberAnimation { property: "x"; from: (control.mirrored ? -0.5 : 0.5) * control.width; to: 0; duration: DS.Style.stackView.animationDuration; easing.type: Easing.OutCubic }
        NumberAnimation { property: "opacity"; from: 0.0; to: 1.0; duration: DS.Style.stackView.animationDuration; easing.type: DS.Style.stackView.animationEasingType }
    }

    replaceExit: Transition {
        // slide_out_left
        NumberAnimation { property: "x"; from: 0; to: (control.mirrored ? -0.5 : 0.5) * -control.width; duration: DS.Style.stackView.animationDuration; easing.type: Easing.OutCubic }
        NumberAnimation { property: "opacity"; from: 1.0; to: 0.0; duration: DS.Style.stackView.animationDuration; easing.type: DS.Style.stackView.animationEasingType }
    }
}
