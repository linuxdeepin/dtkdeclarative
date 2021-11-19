/*
 * Copyright (C) 2020 ~ 2021 Deepin Technology Co., Ltd.
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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

import QtQuick 2.11
import QtQuick.Controls 2.4
import org.deepin.dtk 1.0 as D

/*
    D.PasswordEdit 控件：
    等同于 dtkwidget 里边的 D.PasswordEdit 控件。
*/

D.LineEdit {
    id: control

    // 暴露给外部的属性
    readonly property bool isEchoMode: control.echoMode === TextInput.Normal
    property alias echoButtonVisible: echoButton.visible

    function toggleEchoMode() {
        if (control.isEchoMode) {
            control.echoMode = TextInput.Password
        } else {
            control.echoMode = TextInput.Normal
        }
    }

    echoMode: TextInput.Password
    clearButtonAnchors.rightMargin: echoButton.visible? 8 + echoButton.width : 0

    Button {
        id: echoButton

        anchors {
            right: control.right
            verticalCenter: control.verticalCenter
        }
        width: height
        focusPolicy: Qt.NoFocus
        palette {
            button: D.DTK.themeType === D.ApplicationHelper.LightType ?
                                        control.palette.highlight :
                                        D.DTK.adjustColor(control.palette.highlight, 0, 0, +10, 0, 0, 0, 0)
            text: control.palette.highlightedText
        }
        onClicked: {
            control.toggleEchoMode()
        }

        D.Icon {
            anchors.centerIn: parent
            color: control.palette.base
            name: control.isEchoMode ? "password_hide" : "password_show"
        }
    }
}