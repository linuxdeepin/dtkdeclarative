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
        normal.common: "#f7f7f7"
        hovered.common: "#e1e1e1"
        pressed.common: "#bcc4d0"
    }
    property D.Palette button2: D.Palette {
        objectName: "button2"
        normal.common: "#f0f0f0"
        hovered.common: "#d2d2d2"
        pressed.common: "#cdd6e0"
    }
    property D.Palette buttonText: D.Palette {
        objectName: "buttonText"
        normal.common: Qt.rgba(0, 0, 0, 0.7)
        pressed.common: "#0081ff"
    }
    property D.Palette warningButtonText: D.Palette {
        objectName: "buttonText"
        normal.common: "#ff5736"
    }
    property D.Palette suggestButtonText: D.Palette {
        objectName: "buttonText"
        normal.common: Qt.rgba(1, 1, 1, 0.9)
        hovered.common: "white"
        pressed.common: Qt.rgba(1, 1, 1, 0.6)
    }
    // PixelMetric
    property int buttonIconWidth: 24
    property int buttonIconHeight: 24
    property int controlBackgroundRadius: 8
    property QtObject settings: QtObject {
        property int titleMarginL1: 10
        property int titleMarginL2: 30
        property int titleMarginLOther: 50
        property int contentMarginL1: 10
        property int contentMarginL2: 30
        property int contentMarginOther: 50
    }
}
