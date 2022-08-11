// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

import QtQuick 2.11
import QtQuick.Window 2.11
import QtQuick.Layouts 1.11
import org.deepin.dtk.style 1.0 as DS
import "private"

FocusScope {
    id: control
    property int maxVisibleItems : DS.Style.arrowListView.maxVisibleItems
    property int itemHeight:  DS.Style.arrowListView.itemHeight
    property alias view: itemsView

    implicitWidth: DS.Style.arrowListView.width
    implicitHeight: contentLayout.implicitHeight

    ColumnLayout {
        id: contentLayout
        anchors.fill: parent
        ArrowListViewButton {
            visible: itemsView.interactive
            Layout.alignment: Qt.AlignHCenter
            Layout.preferredWidth: width
            Layout.preferredHeight: height
            view: itemsView
            direction: ArrowListViewButton.UpButton
        }

        ListView {
            id: itemsView
            Layout.fillWidth: true
            Layout.fillHeight: true
            implicitHeight: Math.min(contentHeight, maxVisibleItems * itemHeight)
            interactive: Window.window ? (contentHeight > Window.window.height || model.count > maxVisibleItems) : false
            ScrollIndicator.vertical: ScrollIndicator { }
        }

        ArrowListViewButton {
            visible: itemsView.interactive
            Layout.alignment: Qt.AlignHCenter
            Layout.preferredWidth: width
            Layout.preferredHeight: height
            view: itemsView
            direction: ArrowListViewButton.DownButton
        }
    }
}
