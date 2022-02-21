/*
 * Copyright (C) 2022 UnionTech Technology Co., Ltd.
 *
 * Author:     xiaoyaobing <xiaoyaobing@uniontech.com>
 *
 * Maintainer: xiaoyaobing <xiaoyaobing@uniontech.com>
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
import QtQuick.Controls 2.4
import org.deepin.dtk.impl 1.0 as D
import org.deepin.dtk.style 1.0 as DS

Control {
    id: control

    property alias text: __label.text
    property int textHorizontalAlignment: Text.AlignHCenter
    readonly property int direction: parent.parent.tickDirection
    readonly property bool horizontal: parent.parent.children[0].horizontal

    property D.Palette tickColor: DS.Style.sliderTick

    implicitWidth: horizontal ? DS.Style.slider.tickWidth : __rect.width + __label.width
    implicitHeight: horizontal ? __rect.height + __label.height : DS.Style.slider.tickWidth

    Rectangle {
        id: __rect
        anchors.bottom: horizontal ? (TipsSlider.TickDirection.Front === direction ? parent.bottom : undefined) : undefined
        anchors.right: horizontal ? undefined : (TipsSlider.TickDirection.Front === direction ? parent.right : undefined)
        anchors.left: horizontal ? undefined : (TipsSlider.TickDirection.Back === direction ? parent.left : undefined)
        width: horizontal ? DS.Style.slider.tickWidth : DS.Style.slider.tickHeight
        height: horizontal ? DS.Style.slider.tickHeight : DS.Style.slider.tickWidth
        color: control.D.ColorSelector.tickColor
    }

    Label {
        id: __label
        anchors {
            top: horizontal ? (TipsSlider.TickDirection.Back === direction ? __rect.bottom : undefined) : undefined
            bottom: horizontal ? (TipsSlider.TickDirection.Front === direction ? __rect.top : undefined) : undefined
            left: horizontal ? (Text.AlignLeft === textHorizontalAlignment ? __rect.left : undefined) : (TipsSlider.TickDirection.Back === direction ? __rect.right : undefined)
            right: horizontal ? (Text.AlignRight === textHorizontalAlignment ? __rect.right : undefined) : (TipsSlider.TickDirection.Front === direction ? __rect.left : undefined)
            horizontalCenter: horizontal && Text.AlignHCenter === textHorizontalAlignment ? __rect.horizontalCenter : undefined
            verticalCenter: horizontal ? undefined : __rect.verticalCenter
        }
        width: text ? implicitWidth : 0
        height: text ? implicitHeight : 0
        horizontalAlignment: textHorizontalAlignment
        verticalAlignment: Text.AlignVCenter
    }
}
