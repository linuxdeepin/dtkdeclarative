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
        normal: "#f7f7f7"
        hovered: "#e1e1e1"
        pressed: "#bcc4d0"
    }
    property D.Palette button2: D.Palette {
        normal: "#f0f0f0"
        hovered: "#d2d2d2"
        pressed: "#cdd6e0"
    }
    property D.Palette suggestButton1: D.Palette {
        normal: "#00aaff"
        hovered: "#24b6ff"
        pressed: "#005bcb"
    }
    property D.Palette suggestButton2: D.Palette {
        normal: "#006eff"
        hovered: "#3586ff"
        pressed: "#0067ff"
    }
    property D.Palette buttonText: D.Palette {
        normal: Qt.rgba(0, 0, 0, 0.7)
        pressed: "#0081ff"
    }
    property D.Palette warningButtonText: D.Palette {
        normal: "#ff5736"
    }
    property D.Palette suggestButtonText: D.Palette {
        normal: Qt.rgba(1, 1, 1, 0.9)
        hovered: "white"
        pressed: Qt.rgba(1, 1, 1, 0.6)
    }
    property D.Palette buttonBorder: D.Palette {
        normal: Qt.rgba(0, 0, 0, 0.03)
    }
    property D.Palette suggestButtonBorder: D.Palette {
        normal: Qt.rgba(0, 0.581, 1, 0.2)
    }
    property D.Palette windowButton: D.Palette {
        normal: "transparent"
        normalDark: "transparent"
        hovered: Qt.rgba(0, 0, 0, 0.29)
        hoveredDark: Qt.rgba(0, 0, 0, 0.29)
        pressed: Qt.rgba(0, 0, 0, 0.29)
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
        property int width: 204
        property int height: 36
        property int hPadding: 10
        property int vPadding: 6
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
        property int maximumWidth: 450
        property int minimumHeight: 40
        property int radius: 14
        property color lightBackground: Qt.rgba(247, 247, 247, 0.6)
        property color darkBackground: Qt.rgba(247, 247, 247, 0.6)
        property color lightShadowBackground: Qt.rgba(0, 0, 0, 0.2)
        property color darkShadowBackground: Qt.rgba(0, 0, 0, 0.2)
    }
}
