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

    // Fixed per show: rows realized while scrolling can differ in height.
    property real _measuredRowHeight: 0
    readonly property real _rowHeight: _measuredRowHeight > 0 ? _measuredRowHeight : itemHeight

    // Full height of the list, to size a popup before it is mapped.
    readonly property real sizedHeight: {
        if (itemsView.count === 0)
            return 0
        // Once a delegate exists, zero is a valid measured content height.
        if (itemsView.contentHeight > 0 || itemsView.hasDelegate)
            return implicitHeight
        const arrowStrips = itemsView.count > maxVisibleItems
                ? 2 * (DS.Style.arrowListView.stepButtonIconSize.height + contentLayout.spacing) : 0
        return Math.min(itemsView.count, maxVisibleItems) * _rowHeight + arrowStrips
    }

    implicitWidth: Math.max(DS.Style.arrowListView.width, contentLayout.implicitWidth)
    implicitHeight: contentLayout.implicitHeight

    onVisibleChanged: {
        if (!visible)
            return
        itemsView.itemMaxWidth = 0
        _measuredRowHeight = 0
        itemsView.refreshContentItemWidth()
    }

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
            property bool hasDelegate: false
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
                control._measuredRowHeight = 0
                refreshContentItemWidth()
            }

            onContentHeightChanged: if (control._measuredRowHeight <= 0) refreshContentItemWidth()

            onWidthChanged: refreshContentItemWidth()

            function refreshContentItemWidth() {
                var maxWidth = itemMaxWidth
                var foundDelegate = false
                for (var i = 0; i < itemsView.count; ++i) {
                    var item = itemsView.itemAtIndex(i)
                    if (!item)
                        continue
                    foundDelegate = true
                    if (!item.visible)
                        continue
                    item.width = itemsView.width
                    if (control._measuredRowHeight <= 0 && item.height > 0)
                        control._measuredRowHeight = item.height
                    if (item.implicitWidth > maxWidth)
                        maxWidth = item.implicitWidth
                }
                hasDelegate = foundDelegate
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
