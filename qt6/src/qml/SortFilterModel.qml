// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

import QtQuick
import QtQml.Models 2.11

DelegateModel {
    id: delegateModel

    property var lessThan: function(left, right) { return true; }
    property var filterAcceptsItem: function(item) { return true; }
    property alias visibleGroup: visibleItems

    function update() {

        // Step 1: Filter items
        var visible = [];
        var unVisible = []
        for (var i = 0; i < items.count; ++i) {
            var item = items.get(i);
            if (filterAcceptsItem(item.model)) {
                visible.push(item);
            } else {
                unVisible.push(item);
            }
        }

        // Step 2: Sort the list of visible items
        if (lessThan) {
            visible.sort(function(a, b) {
                return lessThan(a.model, b.model) ? -1 : 1;
            });
        }

        // Step 3: Remove the visible group
        for (i = 0; i < unVisible.length; ++i) {
            items.removeGroups(unVisible[i], 1, "visible")
        }

        // Step 4: Add the visible group
        for (i = 0; i < visible.length; ++i) {
            item = visible[i];
            items.addGroups(visible[i], 1, "visible")
            if (item.visibleIndex !== i) {
                visibleItems.move(item.visibleIndex, i, 1);
            }
        }
    }

    items.onChanged: update()
    onLessThanChanged: update()
    onFilterAcceptsItemChanged: update()

    groups: DelegateModelGroup {
        id: visibleItems

        name: "visible"
        includeByDefault: false
    }

    filterOnGroup: "visible"
}
