/*
 * Copyright (C) 2021 Uniontech Technology Co., Ltd.
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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

import QtQuick 2.11
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.11
import org.deepin.dtk 1.0
import org.deepin.dtk.settings 1.0 as Settings

Settings.OptionDelegate {
    id: control

    property alias impl: impl
    property string valueRole
    property alias model: impl.model

    ComboBox {
        id: impl
        Layout.alignment: Qt.AlignRight
        Layout.fillWidth: true

        currentIndex: {
            var index = -1
            for(var i = 0; i < model.length; i++) {
                var item
                if (valueRole === "") {
                    item = model[i]
                } else {
                    item = model[i][valueRole]
                }
                if (item === Settings.SettingsOption.value) {
                    index = i
                    break
                }
            }
            return index
        }
        onActivated: {
            if (valueRole === "") {
                Settings.SettingsOption.value = control.model[currentIndex]
            } else {
                Settings.SettingsOption.value = control.model[currentIndex][valueRole]
            }
        }
    }
}
