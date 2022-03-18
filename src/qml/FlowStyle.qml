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
            crystal: Qt.rgba(0, 0, 0, 0.1)
        }
        hovered {
            common: "#e1e1e1"
            crystal:  Qt.rgba(16.0 / 255, 16.0 / 255, 16.0 / 255, 0.2)
        }
        pressed {
            common: "#bcc4d0"
            crystal: Qt.rgba(16.0 / 255, 16.0 / 255, 16.0 / 255, 0.15)
        }
    }
    property D.Palette button2: D.Palette {
        normal {
            common: "#f0f0f0"
            crystal: Qt.rgba(0, 0, 0, 0.1)
        }
        hovered {
            common: "#d2d2d2"
            crystal: Qt.rgba(16.0 / 255, 16.0 / 255, 16.0 / 255, 0.2)
        }
        pressed {
            common: "#cdd6e0"
            crystal: Qt.rgba(16.0 / 255, 16.0 / 255, 16.0 / 255, 0.15)
        }
    }
    property D.Palette buttonDropShadow: D.Palette {
        normal: Qt.rgba(0, 0, 0, 0.05)
        hovered: Qt.rgba(0, 0, 0, 0.1)
    }
    property D.Palette buttonInnerShadow1: D.Palette {
        normal: Qt.rgba(0, 0, 0, 0.05)
        pressed: "transparent"
    }
    property D.Palette buttonInnerShadow2: D.Palette {
        normal: Qt.rgba(1, 1, 1, 0.2)
        hovered: Qt.rgba(1, 1, 1, 0.5)
        pressed: "transparent"
    }
    property D.Palette highlightedButton1: D.Palette {
        normal {
            common: "#00aaff"
            crystal: Qt.rgba(0, 0, 0, 0.1)
        }
        hovered {
            common: "#24b6ff"
            crystal: "red"//Qt.rgba(16.0 / 255, 16.0 / 255, 16.0 / 255, 0.2)
        }
        pressed {
            common: "#005bcb"
            crystal: Qt.rgba(16.0 / 255, 16.0 / 255, 16.0 / 255, 0.15)
        }
    }
    property D.Palette highlightedButton2: D.Palette {
        normal {
            common: "#006eff"
            crystal: Qt.rgba(0, 0, 0, 0.1)
        }
        hovered {
            common: "#3586ff"
            crystal: "red"//Qt.rgba(16.0 / 255, 16.0 / 255, 16.0 / 255, 0.2)
        }
        pressed {
            common: "#0067ff"
            crystal: Qt.rgba(16.0 / 255, 16.0 / 255, 16.0 / 255, 0.15)
        }

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
        pressed {
            common: D.DTK.makeColor(D.Color.Highlight)
            crystal: D.DTK.makeColor(D.Color.Highlight)
        }
    }
    property D.Palette warningButtonText: D.Palette {
        normal: "#ff5736"
    }
    property D.Palette highlightedButtonText: D.Palette {
        normal {
            common: Qt.rgba(1, 1, 1, 0.9)
            crystal: D.DTK.makeColor(D.Color.Highlight)
        }
        hovered: "white"
        pressed: Qt.rgba(1, 1, 1, 0.6)
    }
    property D.Palette buttonInsideBorder: D.Palette {
        normal {
            common: Qt.rgba(1, 1, 1, 0.1)
            crystal: Qt.rgba(1, 1, 1, 0.1)
        }
        hovered: Qt.rgba(1, 1, 1, 0.2)
        pressed: Qt.rgba(0, 0, 0, 0.03)
    }
    property D.Palette buttonOutsideBorder: D.Palette {
        normal {
            common: Qt.rgba(0, 0, 0, 0.08)
            crystal: Qt.rgba(0, 0, 0, 0.08)
        }
        hovered: Qt.rgba(0, 0, 0, 0.2)
        pressed: "transparent"
    }
    property D.Palette highlightedButtonBorder: D.Palette {
        normal {
            common: Qt.rgba(0, 129 / 255.0, 1, 0.2)
            crystal: Qt.rgba(1, 1, 1, 0.1)
        }
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
    property D.Palette spinBoxIndicatorBackground: D.Palette {
        normal: Qt.rgba(0, 0, 0, 0.7)
        hovered: Qt.rgba(0, 0, 0, 0.6)
        pressed: Qt.rgba(0, 0, 0, 0.8)
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
        normal: D.DTK.makeColor(D.Color.Highlight)
        hovered: D.DTK.makeColor(D.Color.Highlight).lightness(+10)
    }
    property D.Palette highlightPanelDropShadow:  D.Palette {
        normal: D.DTK.makeColor(D.Color.Highlight).lightness(+20)
    }
    property D.Palette highlightPanelInnerShadow:  D.Palette {
        normal: D.DTK.makeColor(D.Color.Highlight).lightness(-20)
    }

    property D.Palette switchBackground: D.Palette {
        normal: Qt.rgba(50 / 255, 50 / 255, 50 / 255, 0.2)
        normalDark: "#1A1A1A"
    }

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
        property real focusBorderWidth: 2
        property real focusBorderPaddings: 1

        function implicitWidth(control) {
            return Math.max(backgroundImplicitWidth(control), contentImplicitWidth(control))
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
        function backgroundImplicitWidth(control) {
            var background = control.implicitBackgroundWidth !== undefined
                    ? control.implicitBackgroundWidth
                    : (control.background ? control.background.implicitWidth : 0)

            if (control.leftInset !== undefined)
                background += (control.leftInset + control.rightInset)

            return background
        }
        function contentImplicitWidth(control) {
            var content = control.implicitContentWidth !== undefined
                    ? control.implicitContentWidth
                    : (control.contentItem ? control.contentItem.implicitWidth : 0)
            content += (control.leftPadding + control.rightPadding)

            return content
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
        property int contentMargin: 10
        property int contentMarginL1: 10
        property int contentMarginL2: 30
        property int contentMarginOther: 50
        property int navigationWidth: 190
        property int navigationHeight: 20
        property int navigationMargin: 10
        property int navigationTextVPadding: 10
        property D.Palette background: D.Palette {
            normal: "transparent"
            hovered: Qt.rgba(0, 0, 0, 0.1)
        }
    }

    property QtObject edit: QtObject {
        property int width: 180
        property int actionIconSize: 22
        property int textFieldHeight: 36
        property int textAreaHeight: 100
    }

    property QtObject searchEdit: QtObject {
        property int iconSize: 16
        property int iconLeftMargin: 10
        property int iconRightMargin: 7
        property int animationDuration: 200
    }

    property QtObject spinBox: QtObject {
        property int width : 300
        property int height: 36
        property int spacing: 10
        property int focusIndicatorIconSize: 10
        property int indicatorWidth: 24
        property int indicatorHeight: 14
        property int indicatorIconSize: 24
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
        property int closeButtonSize: 22
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
        property int buttonWidth: 50
        property int buttonHeight: 50
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
        property int handleWidth: 30
        property int handleHeight: 24
        property string iconNmae: "switch_button"
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
        property int highlightMargin: -4
        property int tickTextMargin: 2
    }

    property QtObject scrollBar: QtObject {
        property int padding: 2
        property int width: 6
        property int activeWidth: 12
        property color lightBackground: "black"
        property color darkBackground: "black"
        property color innerShadowColor: Qt.rgba(0, 0, 0, 0.5)
        property color outerShadowColor: Qt.rgba(1, 1, 1, 0.1)
        property real hideOpacity: 0.0
        property real normalOpacity: 0.4
        property real hoverOpacity: 0.7
        property real activeOpacity: 0.5
        property int hidePauseDuration: 450
        property int hideDuration: 1500
    }

    property QtObject comboBox: QtObject {
        property int width: 240
        property int height: 36
        property int padding: 8
        property int spacing: 10
        property int iconSize: 16
        property int editableIndicatorSpacing: 7
        property color editableLineColor: Qt.rgba(0, 0, 0, 0.05)
        property int editableIndicatorSize: 22
    }

    property QtObject itemDelegate: QtObject {
        property int width: 204
        property int height: 40
        property color normalColor: Qt.rgba(0, 0, 0, 0.05)
        property color cascadeColor: Qt.rgba(0, 0, 0, 0.15)
        property int iconSize: 22
        property int checkIndicatorIconSize: 22
        property D.Palette checkBackgroundColor: D.Palette {
            hovered: Qt.rgba(0, 0, 0, 0.1)
            normal: Qt.rgba(0, 0, 0, 0.05)
        }
        property color checkedColor: Qt.rgba(0, 0, 0, 0.15)
    }

    property QtObject checkBox: QtObject {
        property int indicatorWidth: 16
        property int indicatorHeight: 16
        property int padding: 2
        property int iconSize: 16
        property int focusRadius: 4
    }

    property QtObject ipLineEdit: QtObject {
        property int fieldWidth: 40
    }

    property QtObject arrowRectangleBlur: QtObject {
        property int arrowHeightFactor: 10
        property int borderWidth: 1
        property int blurRadius: 20
        property color borderColor: Qt.rgba(0, 0, 0, 0.05)
        property color darkBorderColor: Qt.rgba(1, 1, 1, 0.05)
        property color backgroundColor: "#55ffffff"
        property color darkBackgroundColor: "#55000000"
        property color shadowColor: Qt.rgba(0, 0, 0, 0.4)
        property color darkShadowColor: Qt.rgba(1, 1, 1, 0.4)
    }

    function selectColor(conditionColor, lightColor, darkColor) {
        if (conditionColor) {
            return D.DTK.selectColor(conditionColor, lightColor, darkColor)
        }

        return D.DTK.themeType === D.ApplicationHelper.DarkType ? darkColor  : lightColor
    }

    property QtObject floatingButton: QtObject {
        property int size: 24
    }

    property QtObject iconButton: QtObject {
        property int backgroundSize: 36
        property int iconSize: 18
        property int padding: 9
    }

    property QtObject buttonBox: QtObject {
        property int width: 80
        property int height: 36
        property int buttonWidth: 30
        property int buttonHeight: 30
        property int padding: 3
        property int spacing: 6
    }
}
