/*
 * Copyright (C) 2021 ~ 2022 UnionTech Technology Co., Ltd.
 *
 * Author:     JiDe Zhang <zhangjide@deepin.org>
 *
 * Maintainer: JiDe Zhang <zhangjide@deepin.org>
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

import QtQuick 2.0
import org.deepin.dtk.impl 1.0 as D

QtObject {
    property D.Palette button1: D.Palette {
        normal {
            common: "#f7f7f7"
            crystal: Qt.rgba(16.0 / 255, 16.0 / 255, 16.0 / 255, 0.1)
        }
        hovered: "#e1e1e1"
        pressed: "#bcc4d0"
    }
    property D.Palette button2: D.Palette {
        normal {
            common: "#f0f0f0"
            crystal: Qt.rgba(16.0 / 255, 16.0 / 255, 16.0 / 255, 0.1)
        }
        hovered: "#d2d2d2"
        pressed: "#cdd6e0"
    }
    property D.Palette buttonDropShadow: D.Palette {
        normal: Qt.rgba(0, 0, 0, 0.08)
        hovered: Qt.rgba(0, 0, 0, 0.1)
    }
    property D.Palette buttonInnerShadow1: D.Palette {
        normal: Qt.rgba(0, 0, 0, 0.05)
        pressed: "transparent"
    }
    property D.Palette buttonInnerShadow2: D.Palette {
        normal: "transparent"
        hovered: Qt.rgba(1, 1, 1, 0.5)
    }
    property D.Palette highlightedButton1: D.Palette {
        normal: "#00aaff"
        hovered: "#24b6ff"
        pressed: "#005bcb"
    }
    property D.Palette highlightedButton2: D.Palette {
        normal: "#006eff"
        hovered: "#3586ff"
        pressed: "#0067ff"
    }
    property D.Palette highlightedButtonDropShadow: D.Palette {
        normal: Qt.rgba(0, 112 / 255.0, 1, 0.3)
    }
    property D.Palette highlightedButtonInnerShadow1: D.Palette {
        normal: Qt.rgba(0, 50 / 255.0, 1, 0.3)
        hovered: Qt.rgba(0, 85 / 255.0, 1, 0.5)
        pressed: Qt.rgba(0, 97 / 255.0, 1, 0.5)
    }
    property D.Palette highlightedButtonInnerShadow2: D.Palette {
        normal: "transparent"
    }
    property D.Palette buttonText: D.Palette {
        normal {
            common: Qt.rgba(0, 0, 0, 0.7)
            crystal: Qt.rgba(0, 0, 0, 0.9)
        }
        pressed: D.DTK.makeColor(D.Color.Highlight)
    }
    property D.Palette warningButtonText: D.Palette {
        normal: "#ff5736"
    }
    property D.Palette highlightedButtonText: D.Palette {
        normal: Qt.rgba(1, 1, 1, 0.9)
        hovered: "white"
        pressed: Qt.rgba(1, 1, 1, 0.6)
    }
    property D.Palette buttonBorder: D.Palette {
        normal: Qt.rgba(0, 0, 0, 0.03)
        hovered: Qt.rgba(0, 0, 0, 0.15)
    }
    property D.Palette highlightedButtonBorder: D.Palette {
        normal: Qt.rgba(0, 0.581, 1, 0.2)
    }

    property D.Palette checkedButton : D.Palette {
        normal: D.DTK.makeColor(D.Color.Highlight)
        hovered: D.DTK.makeColor(D.Color.Highlight).lightness(+10)
        pressed: D.DTK.makeColor(D.Color.Highlight).lightness(-10)
    }

    property D.Palette checkedButtonText : D.Palette {
        normal: D.DTK.makeColor(D.Color.HighlightedText)
        hovered: D.DTK.makeColor(D.Color.HighlightedText).lightness(+10)
        pressed: D.DTK.makeColor(D.Color.HighlightedText).opacity(-20)
    }

    property D.Palette checkedButtonDropShadow : D.Palette {
        normal: D.DTK.makeColor(D.Color.Highlight).opacity(-60)
        pressed: D.DTK.makeColor(D.Color.Highlight).opacity(-80)
    }

    property D.Palette checkedButtonInnerShadow : D.Palette {
        normal: D.DTK.makeColor(D.Color.Highlight).lightness(-10)
        hovered: D.DTK.makeColor(D.Color.Highlight)
        pressed: D.DTK.makeColor(D.Color.Highlight).lightness(-20)
    }

    property QtObject windowButton: QtObject {
        property int width: 45
        property int height: 45
        property D.Palette background: D.Palette {
            normal: "transparent"
            normalDark: "transparent"
            hovered: Qt.rgba(0, 0, 0, 0.29)
            hoveredDark: Qt.rgba(0, 0, 0, 0.29)
            pressed: Qt.rgba(0, 0, 0, 0.29)
        }
    }
    property D.Palette editBackground: D.Palette {
        normal: Qt.rgba(0, 0, 0, 0.08)
    }
    property D.Palette spinBoxIndicator: D.Palette {
        normal: "white"
        pressed: Qt.rgba(1, 1, 1, 0.6)
    }
    property D.Palette spinBoxIndicatorBackground: D.Palette {
        normal: Qt.rgba(0, 0, 0, 0.7)
    }
    property D.Palette warningText: D.Palette {
        normal: "#e15736"
    }
    property D.Palette tooltipBase: D.Palette {
        normal: Qt.rgba(247, 247, 247, 0.6)
    }
    property D.Palette alertBackground: D.Palette {
        normal: Qt.rgba(0.95, 0.22, 0.20, 0.15)
    }

    property D.Palette highlightPanelBackground: D.Palette {
        normal: "transparent"
        hovered: D.DTK.makeColor(D.Color.Highlight)
        pressed: D.DTK.makeColor(D.Color.Highlight)
    }
    property D.Palette highlightPanelDropShadow:  D.Palette {
        normal: "transparent"
        hovered: D.DTK.makeColor(D.Color.Highlight).lightness(+20)
        pressed: D.DTK.makeColor(D.Color.Highlight).lightness(+20)
    }
    property D.Palette highlightPanelInnerShadow:  D.Palette {
        normal: "transparent"
        hovered: D.DTK.makeColor(D.Color.Highlight).lightness(-20)
        pressed: D.DTK.makeColor(D.Color.Highlight).lightness(-20)
    }

    property D.Palette switchBackground: D.Palette {
        normal: Qt.rgba(50 / 255, 50 / 255, 50 / 255, 0.2)
        normalDark: "#1A1A1A"
    }

    // TODO(Xiao Yao Bing): Replace handle with dci icon. delete this code
    property D.Palette switchHandle: D.Palette {
        normal: "#8c8c8c"
        normalDark: "#444444"
    }

    property D.Palette sliderGroove: D.Palette {
        normal: Qt.rgba(0, 0, 0, 0.2)
        normalDark: Qt.rgba(1, 1, 1, 0.2)
    }

    property D.Palette sliderTick: D.Palette {
        normal: Qt.rgba(0, 0, 0, 0.4)
        normalDark: Qt.rgba(1, 1, 1, 0.4)
    }

    // PixelMetric
    property QtObject control: QtObject {
        property int radius: 8
        property int spacing: 6
        property int padding: 6
        property int borderWidth: 1
        property color lightBorder: Qt.rgba(0, 0, 0, 0.05)
        property color darkBorder: Qt.rgba(0, 0, 0, 0.05)
        property int focusBorderWidth: 2

        function implicitWidth(control) {
            var background = control.implicitBackgroundWidth !== undefined
                    ? control.implicitBackgroundWidth
                    : (control.background ? control.background.implicitWidth : 0)

            if (control.leftInset !== undefined)
                background += (control.leftInset + control.rightInset)

            var content = control.implicitContentWidth !== undefined
                    ? control.implicitContentWidth
                    : (control.contentItem ? control.contentItem.implicitWidth : 0)
            content += (control.leftPadding + control.rightPadding)

            return Math.max(background, content)
        }
        function implicitHeight(control) {
            var background = control.implicitBackgroundHeight !== undefined
                    ? control.implicitBackgroundHeight
                    : (control.background ? control.background.implicitHeight : 0)

            if (control.topInset !== undefined)
                background += (control.topInset + control.bottomInset)

            var content = control.implicitContentHeight !== undefined
                    ? control.implicitContentHeight
                    : (control.contentItem ? control.contentItem.implicitHeight : 0)
            content += (control.topPadding + control.bottomPadding)

            return Math.max(background, content)
        }
    }

    property QtObject button: QtObject {
        property int width: 140
        property int height: 36
        property int hPadding: control.radius
        property int vPadding: control.radius / 2.0
        property int iconSize: 24
    }

    property QtObject settings: QtObject {
        property int titleMarginL1: 10
        property int titleMarginL2: 30
        property int titleMarginLOther: 50
        property int contentMarginL1: 10
        property int contentMarginL2: 30
        property int contentMarginOther: 50
    }

    property QtObject edit: QtObject {
        property int width: 180
        property int textFieldHeight: 36
        property int textAreaHeight: 100
    }

    property QtObject searchEdit: QtObject {
        property int iconLeftMargin: 10
        property int iconRightMargin: 7
        property int animationDuration: 200
    }

    property QtObject spinBox: QtObject {
        property int width : 300
        property int height: 36
        property int spacing: 10
        property int indicatorSpacing: 4
        property int indicatorWidth: 24
        property int indicatorHeight: 14
        property int indicatorIconSize: 10
    }

    property QtObject plusMinusSpinBox: QtObject {
        property int buttonIconSize: 16
    }

    property QtObject floatingPanel: QtObject {
        property int width: 180
        property int height: 40
        property int radius: 14
        property color lightBackground: Qt.rgba(247 / 255.0, 247 / 255.0, 247 / 255.0, 0.6)
        property color darkBackground: Qt.rgba(247 / 255.0, 247 / 255.0, 247 / 255.0, 0.6)
        property color lightShadowBackground: Qt.rgba(0, 0, 0, 0.2)
        property color darkShadowBackground: Qt.rgba(0, 0, 0, 0.2)
    }

    property QtObject floatingMessage: QtObject {
        property int maximumWidth: 450
        property int minimumHeight: 40
    }

    property QtObject alertToolTip: QtObject {
        property int connectorWidth: 3
        property int connectorHeight: 12
        property int verticalPadding: 4
        property int horizontalPadding: 10
    }

    property QtObject highlightPanel: QtObject {
        property int width: 180
        property int height: 30
    }

    property QtObject arrowListView: QtObject {
        property size stepButtonSize: Qt.size(16, 16)
        property int maxVisibleItems: 16
        property int itemHeight: 30
    }

    property QtObject menu: QtObject {
        property int padding: 10
        property int radius: 18
        property int margins: 0
        property int overlap: 1
        property color lightBackground: Qt.rgba(235 / 255.0, 235 / 255.0, 235 / 255.0, 0.6)
        property color darkBackground: Qt.rgba(247 / 255.0, 247 / 255.0, 247 / 255.0, 0.6)
        property int itemWidth: 180
        property int itemHeight: 30
        property size itemIconSize: Qt.size(14, 14)
        property color subMenuOpendBackground: Qt.rgba(0, 0, 0, 0.15)
        property int itemCount: 0
        property int separatorLineTopPadding : 6
        property int separatorLineBottomPadding : 4
        property int separatorLineHeight : 2
        property color separatorLineColor : Qt.rgba(0, 0, 0, 0.1)
        property int separatorTopPadding: 11
        property int separatorBottomPadding: 2
    }

    property QtObject popup: QtObject {
        property int width: 80
        property int height: 180
        property int radius: 18
        property int padding: 10
    }

    property QtObject titleBar: QtObject {
        property int height: 45
    }

    property QtObject behindWindowBlur: QtObject {
        property color lightColor: Qt.rgba(235 / 255.0, 235 / 255.0, 235 / 255.0, 0.6)
        property color lightNoBlurColor: Qt.rgba(235 / 255.0, 235 / 255.0, 235 / 255.0, 1.0)
        property color darkColor: "#55000000"
        property color darkNoBlurColor: Qt.rgba(35 / 255.0, 35 / 255.0, 35 / 255.0, 1.0)
    }

    property QtObject switchButton: QtObject {
        property int indicatorWidth: 50
        property int indicatorHeight: 24
        property int buttonWidth: 30
        property int buttonHeight: 24
    }
    
    property QtObject slider: QtObject {
        property int handleWidth: 20
        property int handleHeight: 24
        property int tickWidth: 1
        property int tickHeight: 12
        property int grooveWidth: 100
        property int grooveHeight: 4
        property int sliderWidth: handleWidth + grooveWidth
        property int sliderHeight: 60
        property int tickRadius: 8
        property int handleRadius: tickRadius
        property int highlightMargin: -4
        property int tickTextMargin: 2
        property int tickTextSize: 11
        property int tickTextHeight: 20
    }
}
