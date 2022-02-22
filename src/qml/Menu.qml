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
    property var model: control.contentModel
    property Component header
    property Component footer
    readonly property bool existsChecked: {
        for (var i = 0; i < count; ++i) {
            var item = itemAt(i)
            if (item && item.checked)
                return true
        }
        return false
    }

    implicitHeight: DS.Style.control.implicitHeight(control)
    implicitWidth: DS.Style.control.implicitWidth(control)
    margins: DS.Style.menu.margins
    overlap: DS.Style.menu.overlap
    padding: DS.Style.menu.padding

    delegate: MenuItem { }

    D.PopupHandle.delegate: PopupWindow {
        blurControl: control
    }

    contentItem: FocusScope {
        implicitHeight: childrenRect.height
        implicitWidth: childrenRect.width
        Column {
            width: parent.width
            Loader {
                width: parent.width
                sourceComponent: control.header
            }
            ArrowListView {
                id: contentView
                property int count: contentView.view.count

                view.model: control.model
                width: parent.width
                view.currentIndex: control.currentIndex
                maxVisibleItems: control.maxVisibleItems
                itemHeight: DS.Style.menu.itemHeight

                function refreshContentItemWidth()
                {
                    for (var i = 0; i < view.count; ++i) {
                        var item = view.model.get(i)
                        item.width = view.width
                    }
                }

                onCountChanged: {
                    refreshContentItemWidth()
                }
            }
            Loader {
                width: parent.width
                sourceComponent: control.footer
            }
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
