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
import QtQuick.Window 2.11
import QtQuick.Layouts 1.11
import org.deepin.dtk.style 1.0 as DS

FocusScope {
    property int maxVisibleItems : DS.Style.arrowListView.maxVisibleItems
    property int itemHeight:  DS.Style.arrowListView.itemHeight
    property alias view: itemsView

    implicitHeight: childrenRect.height
    implicitWidth: childrenRect.width

    Column {
        width: parent.width
        Button {
            width: DS.Style.arrowListView.stepButtonSize.width
            height: DS.Style.arrowListView.stepButtonSize.height
            flat: true
            anchors.horizontalCenter: parent.horizontalCenter
            enabled: !itemsView.atYBeginning
            visible: itemsView.interactive
            icon.name: "arrow_ordinary_up"
            icon.width: DS.Style.arrowListView.stepButtonIconSize.width
            icon.height: DS.Style.arrowListView.stepButtonIconSize.height
            onClicked: itemsView.decrementCurrentIndex()
        }

        ListView {
            id: itemsView
            width: parent.width
            implicitHeight: Math.min(contentHeight, maxVisibleItems * DS.Style.arrowListView.itemHeight)
            interactive: Window.window ? (contentHeight > Window.window.height || model.count > maxVisibleItems) : false
            ScrollIndicator.vertical: ScrollIndicator { }
        }

        Button {
            width: DS.Style.arrowListView.stepButtonSize.width
            height: DS.Style.arrowListView.stepButtonSize.height
            flat: true
            anchors.horizontalCenter: parent.horizontalCenter
            enabled: !itemsView.atYEnd
            visible: itemsView.interactive
            icon.name: "arrow_ordinary_down"
            icon.width: DS.Style.arrowListView.stepButtonIconSize.width
            icon.height: DS.Style.arrowListView.stepButtonIconSize.height
            onClicked: itemsView.incrementCurrentIndex()
        }
    }
}
