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
import org.deepin.dtk.impl 1.0 as D
import org.deepin.dtk.style 1.0 as DS

Control {
    id: control

    property string text
    property int textHorizontalAlignment: Text.AlignHCenter
    readonly property int direction: parent.parent.tickDirection
    readonly property bool horizontal: parent.parent.children[0].horizontal
    property bool highlight

    property D.Palette tickColor: DS.Style.slider.tick.background
    property D.Palette textColor: highlight ? DS.Style.checkedButton.text: DS.Style.button.text

    implicitWidth: horizontal ? DS.Style.slider.tick.width : __rect.width + __label.width
    implicitHeight: horizontal ? __rect.height + __label.height: DS.Style.slider.tick.width

    Rectangle {
        id: __rect
        anchors.bottom: horizontal ? (TipsSlider.TickDirection.Front === direction ? parent.bottom : undefined) : undefined
        anchors.right: horizontal ? undefined : (TipsSlider.TickDirection.Front === direction ? parent.right : undefined)
        anchors.left: horizontal ? undefined : (TipsSlider.TickDirection.Back === direction ? parent.left : undefined)
        width: horizontal ? DS.Style.slider.tick.width : DS.Style.slider.tick.height
        height: horizontal ? DS.Style.slider.tick.height : DS.Style.slider.tick.width
        color: control.D.ColorSelector.tickColor
    }

    Loader {
        id: __label
        active: text.length !== 0
        anchors {
            top: horizontal ? (TipsSlider.TickDirection.Back === direction ? __rect.bottom : undefined) : undefined
            topMargin: horizontal && (TipsSlider.TickDirection.Back === direction) ? DS.Style.slider.tick.vPadding : undefined
            bottom: horizontal ? (TipsSlider.TickDirection.Front === direction ? __rect.top : undefined) : undefined
            bottomMargin: horizontal && (TipsSlider.TickDirection.Front === direction) ? DS.Style.slider.tick.vPadding : undefined
            left: horizontal ? (Text.AlignLeft === textHorizontalAlignment ? __rect.left : undefined)
                             : (TipsSlider.TickDirection.Back === direction ? __rect.right : undefined)
            leftMargin: !horizontal && TipsSlider.TickDirection.Back === direction ? DS.Style.slider.tick.hPadding : undefined
            right: horizontal ? (Text.AlignRight === textHorizontalAlignment ? __rect.right : undefined)
                              : (TipsSlider.TickDirection.Front === direction ? __rect.left : undefined)
            rightMargin: !horizontal && TipsSlider.TickDirection.Front === direction ? DS.Style.slider.tick.hPadding : undefined
            horizontalCenter: horizontal && Text.AlignHCenter === textHorizontalAlignment ? __rect.horizontalCenter : undefined
            verticalCenter: horizontal ? undefined : __rect.verticalCenter
        }

        sourceComponent: Label {
            text: control.text
            rightPadding: highlight ? DS.Style.slider.tick.hPadding : 0
            leftPadding: rightPadding
            topPadding: highlight ?  DS.Style.slider.tick.vPadding : 0
            bottomPadding: topPadding
            horizontalAlignment: textHorizontalAlignment
            verticalAlignment: Text.AlignVCenter
            palette.windowText: control.D.ColorSelector.textColor
            background: Loader {
                active: highlight
                sourceComponent: HighlightPanel { }
            }
        }
    }
}
