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
import com.deepin.dtk 1.0

/*
    DLineEdit 控件：
    等同于 dtkwidget 里边的 DLineEdit 控件。
*/

TextField {
    id: control

    // clearButtonAnchors 属性用于调整按钮的位置，比如在按钮右侧插入控件
    property alias clearButtonAnchors: clearBtn.anchors

    rightPadding: clearBtn.visible? clearBtn.width + clearBtn.anchors.rightMargin : 0

    Button {
        id: clearBtn

        width: height
        anchors {
            right: control.right
            verticalCenter: control.verticalCenter
        }
        visible: control.text.length !== 0
        focusPolicy: Qt.NoFocus
        onClicked: {
            control.clear()
        }

        DIcon {
            anchors.centerIn: parent
            name: "window-close_round"
        }
    }
}
