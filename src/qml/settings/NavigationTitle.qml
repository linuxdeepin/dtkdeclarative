/*
 * Copyright (C) 2021 UnionTech Technology Co., Ltd.
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
import QtQuick.Window 2.11
import org.deepin.dtk.impl 1.0 as D
import org.deepin.dtk.settings 1.0 as Settings
import org.deepin.dtk.style 1.0 as DS

Button {
    id: control
    property var __view: control.ListView
    height: 40
    width: parent.width
    text: Settings.SettingsGroup.name
    font: __getFont(Settings.SettingsGroup.level)
    leftPadding: __getMargin(Settings.SettingsGroup.level)
    anchors.left: parent.left

    highlighted: __view.isCurrentItem

    onClicked: {
        __view.view.currentIndex = Settings.SettingsGroup.index
    }

    function __getFont(level) {
        switch(level) {
        case 0:
            return D.DTK.fontManager.t4
        case 1:
            return D.DTK.fontManager.t5
        }
        return D.DTK.fontManager.t6
    }
    function __getMargin(level) {
        switch(level) {
        case 0:
            return DS.Style.settings.titleMarginL1
        case 1:
            return DS.Style.settings.titleMarginL2
        }
        return DS.Style.settings.titleMarginLOther
    }
}
