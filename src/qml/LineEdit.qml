/*
 * Copyright (C) 2020 ~ 2022 Deepin Technology Co., Ltd.
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
import org.deepin.dtk.impl 1.0 as D
import org.deepin.dtk.style 1.0 as DS

TextField {
    id: control
    property alias clearButtonAnchors: clearBtn.anchors

    rightPadding: clearBtn.visible ? (clearBtn.width + clearBtn.anchors.rightMargin) : 0
    selectByMouse: true

    ActionButton {
        id: clearBtn
        width: height
        height: control.background.implicitHeight
        anchors {
            right: control.right
            verticalCenter: control.verticalCenter
        }
        visible: control.text.length !== 0
        focusPolicy: Qt.NoFocus
        icon {
            name: "entry_clear"
            width: DS.Style.edit.actionIconSize
            height: DS.Style.edit.actionIconSize
        }
        onClicked: {
            control.clear()
        }
    }
}
