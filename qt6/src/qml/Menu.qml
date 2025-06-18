// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

import QtQuick
import QtQuick.Window
import QtQuick.Layouts
import QtQuick.Templates as T
import org.deepin.dtk 1.0 as D
import org.deepin.dtk.style 1.0 as DS

T.Menu {
    id: control

    property bool closeOnInactive: true
    property int maxVisibleItems : DS.Style.arrowListView.maxVisibleItems
    property D.Palette backgroundColor: DS.Style.menu.background
    property D.Palette backgroundNoBlurColor: DS.Style.menu.backgroundNoBlur
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
    readonly property bool active: parent && parent.Window.active

    implicitHeight: DS.Style.control.implicitHeight(control)
    implicitWidth: DS.Style.control.implicitWidth(control)
    margins: DS.Style.menu.margins
    overlap: DS.Style.menu.overlap
    padding: DS.Style.menu.padding
    topPadding: DS.Style.menu.topPadding
    bottomPadding: topPadding

    delegate: MenuItem { }

    D.PopupHandle.delegate: PopupWindow {
        blurControl: control
    }

    contentItem: FocusScope {
        // QTBUG-99897 focus doesn't be clear.
        implicitWidth: viewLayout.implicitWidth
        implicitHeight: viewLayout.implicitHeight
        ColumnLayout {
            id: viewLayout
            spacing: 0
            anchors.fill: parent

            Loader {
                Layout.fillWidth: true
                Layout.preferredHeight: height
                sourceComponent: control.header
            }
            ArrowListView {
                id: contentView
                property int count: contentView.view.count

                view.model: control.model
                Layout.fillWidth: true
                Layout.fillHeight: true
                view.currentIndex: control.currentIndex
                maxVisibleItems: control.maxVisibleItems
                itemHeight: DS.Style.menu.item.height
                view.highlightFollowsCurrentItem: true
                view.highlightMoveDuration: 50
                view.highlightMoveVelocity: -1

                view.highlight: HighlightPanel {
                    id: highlightRect
                    visible: false // TODO listview's highlight is confict with Item's highlight.
                    anchors.left: parent ? parent.left : undefined
                    anchors.right: parent ? parent.right : undefined
                    anchors.leftMargin: 0
                    anchors.rightMargin: 0
                    scale: D.DTK.hasAnimation ? 0.9 : 1.0

                    radius: DS.Style.menu.item.radius
                    outerShadowColor: null
                    innerShadowColor: null
                    Component.onCompleted: {
                        scale = 1.0
                    }
                    Behavior on scale {
                        NumberAnimation { duration: 100 }
                    }
                }
            }
            Loader {
                Layout.fillWidth: true
                Layout.preferredHeight: height
                sourceComponent: control.footer
            }
        }
    }

    background: Loader {
        active: !control.D.PopupHandle.window
        sourceComponent: FloatingPanel {
            implicitWidth: DS.Style.menu.item.width
            implicitHeight: DS.Style.menu.item.height
            radius: DS.Style.menu.radius
            blurMultiplier: 8.0
            backgroundColor: control.backgroundColor
            backgroundNoBlurColor: control.backgroundNoBlurColor
            outsideBorderColor: null
            enableBlur: false // TODO disable blur temporarily, pms:BUG 300055
        }
    }

    onActiveChanged: {
        if (!active && closeOnInactive) {
            control.close()
        }
    }
}
