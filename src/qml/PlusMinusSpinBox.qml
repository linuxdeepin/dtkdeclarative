/*
 * Copyright (C) 2020 ~ 2021 UnionTech Technology Co., Ltd.
 *
 * Author:     Chen Bin <chenbin@uniontech.com>
 *
 * Maintainer: Chen Bin <chenbin@uniontech.com>
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
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.11
import org.deepin.dtk.style 1.0 as DS

RowLayout {
    id: control
    width: DS.Style.spinBox.width
    height: DS.Style.spinBox.height

    // Default SpinBox Properties
    property alias spinBox: spinBoxControl
    // Button Visible Properties
    property alias upButtonVisible: plusButton.visible
    property alias downButtonVisible: minusButton.visible
    property alias resetButtonVisible: resetButton.visible

    spacing: DS.Style.spinBox.spacing
    SpinBox {
        id: spinBoxControl
        Layout.fillWidth: true
        up.indicator: null
        down.indicator: null
    }

    IconButton {
        id: plusButton

        implicitWidth: DS.Style.spinBox.height
        implicitHeight: DS.Style.spinBox.height
        focusPolicy: Qt.NoFocus

        icon.name: "button_add"  // TODO(Chen Bin): Replace it with dci icon.
        icon.width: 16
        onClicked: {
            spinBoxControl.increase()
        }
    }

    IconButton {
        id: minusButton

        implicitWidth: DS.Style.spinBox.height
        implicitHeight: DS.Style.spinBox.height
        focusPolicy: Qt.NoFocus

        icon.name: "button_reduce"  // TODO(Chen Bin): Replace it with dci icon.
        icon.width: 16
        onClicked: {
            spinBoxControl.decrease()
        }
    }

    IconButton {
        id: resetButton

        implicitWidth: DS.Style.spinBox.height
        implicitHeight: DS.Style.spinBox.height
        focusPolicy: Qt.NoFocus

        visible: false
        icon.name: ""  // TODO(Chen Bin): Replace it with dci icon.
        icon.width: 16

        onClicked: {
            spinBoxControl.value = 0
        }
    }
}
