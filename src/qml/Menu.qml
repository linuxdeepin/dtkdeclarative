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

T.Menu {
    id: control

    property int maxVisibleItems : DS.Style.arrowListView.maxVisibleItems

    implicitHeight: DS.Style.control.implicitHeight(control)
    implicitWidth: DS.Style.control.implicitWidth(control)

    margins: DS.Style.menu.margins
    overlap: DS.Style.menu.overlap
    padding: DS.Style.menu.padding

    delegate: MenuItem { }

    D.PopupHandle.delegate: PopupWindow {
        blurControl: control
    }

    contentItem: ArrowListView {
        id: contentView
        property int count: contentView.view.count
        property bool __ImplicitWidthInitialized: false

        view.model: control.contentModel
        view.currentIndex: control.currentIndex
        maxVisibleItems: control.maxVisibleItems
        itemHeight: DS.Style.menu.itemHeight

        function maxItemWith()
        {
            var maxWidth = 0
            for (var i = 0; i < view.count; i++) {
                var item = view.model.get(i)
                maxWidth = Math.max(item.implicitWidth, maxWidth)
            }
            return maxWidth
        }
        function refreshContentItemWidth()
        {
            for (var i = 0; i < view.count; ++i) {
                var item = view.model.get(i)
                item.width = view.width
            }
        }

        onCountChanged: {
            if (!__ImplicitWidthInitialized) {
                view.implicitWidth = Math.max(maxItemWith(), DS.Style.menu.itemWidth)
                __ImplicitWidthInitialized = true
            }
            refreshContentItemWidth()
        }
    }

    background: FloatingPanel {
        visible: !control.D.PopupHandle.window
        implicitWidth: DS.Style.menu.itemWidth
        implicitHeight: DS.Style.menu.itemHeight
        radius: DS.Style.menu.radius
        backgroundColor: D.DTK.selectColor(palette.window, DS.Style.menu.lightBackground, DS.Style.menu.lightBackground)
    }
}
