/*
 * Copyright (C) 2021 UnionTech Technology Co., Ltd.
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
        objectName: "button1"
        normal: "#f7f7f7"
        hovered: "#e1e1e1"
        pressed: "#bcc4d0"
    }
    property D.Palette button2: D.Palette {
        objectName: "button2"
        normal: "#f0f0f0"
        hovered: "#d2d2d2"
        pressed: "#cdd6e0"
    }
    property D.Palette suggestButton1: D.Palette {
        objectName: "button1"
        normal: "#00aaff"
        hovered: "#24b6ff"
        pressed: "#005bcb"
    }
    property D.Palette suggestButton2: D.Palette {
        objectName: "button2"
        normal: "#006eff"
        hovered: "#3586ff"
        pressed: "#0067ff"
    }
    property D.Palette buttonText: D.Palette {
        objectName: "buttonText"
        normal: Qt.rgba(0, 0, 0, 0.7)
        pressed: "#0081ff"
    }
    property D.Palette warningButtonText: D.Palette {
        objectName: "buttonText"
        normal: "#ff5736"
    }
    property D.Palette suggestButtonText: D.Palette {
        objectName: "buttonText"
        normal: Qt.rgba(1, 1, 1, 0.9)
        hovered: "white"
        pressed: Qt.rgba(1, 1, 1, 0.6)
    }
    property D.Palette buttonBorder: D.Palette {
        objectName: "buttonBorder"
        normal: Qt.rgba(0, 0, 0, 0.03)
    }
    property D.Palette suggestButtonBorder: D.Palette {
        objectName: "buttonBorder"
        normal: Qt.rgba(0, 0.581, 1, 0.2)
    }

    // PixelMetric
    property QtObject control: QtObject {
        property int radius: 8
        property int spacing: 6
        property int borderWidth: 1
        property int focusBorderWidth: 2
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

    property QtObject searchEdit: QtObject {
        property int iconLeftMargin: 10
        property int iconRightMargin: 7
        property int animationDuration: 200
    }
}
