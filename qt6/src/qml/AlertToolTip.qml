// SPDX-FileCopyrightText: 2021 - 2026 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

import QtQuick
import QtQuick.Controls
import org.deepin.dtk 1.0 as D
import org.deepin.dtk.style 1.0 as DS

Control {
    id: control
    property Item target
    property string text
    property int timeout: 0
    property bool _expired: false

    // 目标是否已被裁剪出可视区域（例如编辑页向上滚动后输入框已不可见）。
    // 此时提示框不应继续显示，否则会出现输入框已滚走、提示框仍悬浮在页面上的问题。
    // 做法参照 Qt 内部 Qt::ImInputItemClipRectangle 的实现（qquickitem.cpp）：
    // 从目标自身开始逐级向上映射矩形，对每一个 clip 为 true 的祖先求交，
    // 最后再与窗口矩形求交，结果为空即表示目标已完全不可见。
    readonly property bool _targetOutOfView: {
        if (!target || !target.visible || target.opacity <= 0)
            return true

        let rect = Qt.rect(0, 0, target.width, target.height)
        let par = target
        while (par.parent) {
            const parpar = par.parent
            rect = parpar.mapFromItem(par, rect)
            if (parpar.clip)
                rect = _intersectRect(rect, Qt.rect(0, 0, parpar.width, parpar.height))
            if (rect.width <= 0 || rect.height <= 0)
                return true
            par = parpar
        }
        // 映射到场景坐标后与窗口矩形求交。循环结束时 par 即为根项，
        // 也就是窗口的 contentItem，其尺寸等同窗口尺寸。
        rect = par.mapToItem(null, rect)
        rect = _intersectRect(rect, Qt.rect(0, 0, par.width, par.height))
        return rect.width <= 0 || rect.height <= 0
    }

    // QML 的 rect 值类型未暴露 intersected()，这里手动求交
    function _intersectRect(a, b) {
        const x = Math.max(a.x, b.x)
        const y = Math.max(a.y, b.y)
        const right = Math.min(a.x + a.width, b.x + b.width)
        const bottom = Math.min(a.y + a.height, b.y + b.height)
        return Qt.rect(x, y, Math.max(0, right - x), Math.max(0, bottom - y))
    }

    readonly property bool _shown: control.visible && !_expired && !_targetOutOfView

    property point __itemGlobalPos: {
        let x = 0, y = 0
        let a = target
        while (a && a.parent) {
            x += a.x
            y += a.y
            a = a.parent
        }
        return Qt.point(x, y)
    }
    x: __itemGlobalPos.x
    y: __itemGlobalPos.y + (target ? target.height : 0) + DS.Style.control.spacing + __slideOffset
    // 仅显示/隐藏时做滑入滑出动画；滚动或布局变化时 y 立即跟随，
    // 避免动画反复重启导致提示框滞后于输入框
    property real __slideOffset: _shown ? 0 : -DS.Style.control.spacing
    Behavior on __slideOffset {
        NumberAnimation { duration: 200 }
    }
    parent: _shown ? Overlay.overlay : target
    opacity: _shown ? 1 : 0
    enabled: _shown
    topPadding: DS.Style.alertToolTip.verticalPadding
    bottomPadding: DS.Style.alertToolTip.verticalPadding
    leftPadding: DS.Style.alertToolTip.horizontalPadding
    rightPadding: DS.Style.alertToolTip.horizontalPadding
    implicitWidth: target ? Math.min(DS.Style.control.implicitWidth(control), target.width) : DS.Style.control.implicitWidth(control)
    implicitHeight: DS.Style.control.implicitHeight(control)

    Timer {
        id: autoHideTimer
        interval: control.timeout
        running: control.timeout > 0 && control.visible && !control._expired
        onTriggered: control._expired = true
    }

    onVisibleChanged: {
        _expired = false
        if (visible && timeout > 0)
            autoHideTimer.restart()
    }

    onTextChanged: {
        _expired = false
        if (visible && timeout > 0)
            autoHideTimer.restart()
    }

    background: FloatingPanel {
        radius: DS.Style.alertToolTip.radius
        implicitWidth: DS.Style.alertToolTip.width
        implicitHeight: DS.Style.alertToolTip.height
        backgroundColor: DS.Style.alertToolTip.background
        insideBorderColor: DS.Style.alertToolTip.insideBorder
        outsideBorderColor: DS.Style.alertToolTip.outsideBorder
    }

    contentItem: Text {
        property D.Palette textColor: DS.Style.alertToolTip.text
        horizontalAlignment: Text.AlignLeft
        verticalAlignment: Text.AlignVCenter
        text: control.text
        font: control.font
        color: D.ColorSelector.textColor
        wrapMode: Text.Wrap
    }

    BoxShadow {
        id: line
        property D.Palette dropShadowColor: DS.Style.alertToolTip.connecterdropShadow
        property D.Palette backgroundColor: DS.Style.alertToolTip.connecterBackground
        property D.Palette borderColor: DS.Style.control.border
        y: -height * 0.75
        width: DS.Style.alertToolTip.connectorWidth
        height: DS.Style.alertToolTip.connectorHeight
        shadowBlur: 4
        shadowOffsetY: 2
        shadowColor: D.ColorSelector.dropShadowColor
        cornerRadius: DS.Style.control.radius

        Rectangle {
            anchors.fill: parent
            color: line.D.ColorSelector.backgroundColor
            border.color: line.D.ColorSelector.borderColor
            border.width: 1
        }
    }
}
