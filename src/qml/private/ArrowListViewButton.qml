/*
 * Copyright (C) 2022 UnionTech Technology Co., Ltd.
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
import org.deepin.dtk.style 1.0 as DS
import ".."

Loader {
    enum Direction {
        UpButton = 0,
        DownButton = 1
    }

    property Item view
    property int direction
    active: view.interactive

    sourceComponent: Button {
        flat: true
        enabled: !view.atYBeginning
        width: DS.Style.arrowListView.stepButtonSize.width
        height: DS.Style.arrowListView.stepButtonSize.height
        icon.name: direction === ArrowListViewButton.UpButton ? DS.Style.arrowListView.upButtonIconName
                                                              : DS.Style.arrowListView.downButtonIconName
        icon.width: DS.Style.arrowListView.stepButtonIconSize.width
        icon.height: DS.Style.arrowListView.stepButtonIconSize.height
        onClicked: direction === ArrowListViewButton.UpButton ? view.decrementCurrentIndex()
                                                              : view.incrementCurrentIndex()
    }
}
