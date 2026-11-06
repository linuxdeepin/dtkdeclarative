// SPDX-FileCopyrightText: 2022 - 2026 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

import QtQuick
import QtQuick.Window
import QtQuick.Layouts
import org.deepin.dtk 1.0 as D
import org.deepin.dtk.style 1.0 as DS
import org.deepin.dtk.private 1.0 as P

FocusScope {
    id: control
    property int maxVisibleItems : DS.Style.arrowListView.maxVisibleItems
    property int itemHeight:  DS.Style.arrowListView.itemHeight
    property alias view: itemsView

    // Height of a visible realized row, or the style height before one exists:
    // row 0 can be a row hidden by a visible role, and its height is 0.
    readonly property real _rowHeight: {
        if (itemsView.contentHeight > 0) {
            for (var i = 0; i < itemsView.count; ++i) {
                const item = itemsView.itemAtIndex(i)
                if (item && item.visible && item.height > 0)
                    return item.height
            }
        }
        return itemHeight
    }

    // Full height of the list, to size a popup before it is mapped
    readonly property real sizedHeight: {
        if (itemsView.count === 0)
            return 0
        if (itemsView.contentHeight > 0)
            return implicitHeight
        const arrowStrips = itemsView.count > maxVisibleItems
                ? 2 * (DS.Style.arrowListView.stepButtonIconSize.height + contentLayout.spacing) : 0
        return Math.min(itemsView.count, maxVisibleItems) * _rowHeight + arrowStrips
    }

    implicitWidth: Math.max(DS.Style.arrowListView.width, contentLayout.implicitWidth)
    implicitHeight: contentLayout.implicitHeight

    ColumnLayout {
        id: contentLayout
        anchors.fill: parent
        P.ArrowListViewButton {
            visible: itemsView.interactive
            Layout.alignment: Qt.AlignHCenter
            Layout.fillWidth: true
            Layout.preferredHeight: DS.Style.arrowListView.stepButtonIconSize.height
            view: itemsView
            stepSize: control.itemHeight
            direction: P.ArrowListViewButton.UpButton
        }

        ListView {
            id: itemsView
            property real itemMaxWidth: 0
            clip: true
            Layout.fillWidth: true
            Layout.fillHeight: true
            onHeightChanged: if (height > 0 && contentHeight === 0) forceLayout()
            implicitHeight: itemsView.count > 0 ? Math.min(contentHeight, maxVisibleItems * control._rowHeight) : 0
            implicitWidth: itemMaxWidth
            interactive: itemsView.contentHeight > itemsView.height
            ScrollIndicator.vertical: ScrollIndicator { }
            highlight: HighlightPanel {
                visible: false // TODO listview's highlight is confict with Item's highlight.
                outerShadowColor: null
                innerShadowColor: null
                scale: D.DTK.hasAnimation ? 0.9 : 1.0
                Component.onCompleted: {
                    scale = 1.0
                }
                Behavior on scale {
                    NumberAnimation { duration: 100 }
                }
            }
            highlightFollowsCurrentItem: true
            highlightMoveDuration: -1
            highlightMoveVelocity: 400

            Connections {
                target: itemsView.contentItem
                function onChildrenChanged() { Qt.callLater(itemsView.refreshContentItemWidth) }
            }

            onCountChanged: {
                itemMaxWidth = 0
                refreshContentItemWidth()
            }

            onWidthChanged: refreshContentItemWidth()

            function refreshContentItemWidth() {
                var maxWidth = itemMaxWidth
                for (var i = 0; i < itemsView.count; ++i) {
                    var item = itemsView.itemAtIndex(i)
                    if (!item || !item.visible)
                        continue
                    item.width = itemsView.width
                    if (item.implicitWidth > maxWidth)
                        maxWidth = item.implicitWidth
                }
                itemMaxWidth = maxWidth
            }
        }

        P.ArrowListViewButton {
            visible: itemsView.interactive
            Layout.alignment: Qt.AlignHCenter
            Layout.fillWidth: true
            Layout.preferredHeight: DS.Style.arrowListView.stepButtonIconSize.height
            view: itemsView
            stepSize: control.itemHeight
            direction: P.ArrowListViewButton.DownButton
        }
    }
}
