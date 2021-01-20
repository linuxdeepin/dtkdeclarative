/*
 * Copyright (C) 2020 ~ 2020 Deepin Technology Co., Ltd.
 *
 * Author:     liuyang <liuyang@uniontech.com>
 *
 * Maintainer: liuyang <liuyang@uniontech.com>
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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

import QtQuick 2.11
import QtQuick.Shapes 1.11
import QtQuick.Controls 2.4
import QtQuick.Controls.impl 2.4
import QtQuick.Templates 2.4 as T
import "PixelMetric.js" as PM

T.Slider {
    id: control

    enum TickPosition {
        NoTicks = 0,
        TicksAbove = 1,
        TicksLeft = 1,
        TicksBelow = 2,
        TicksRight = 2,
        TicksBothSides = 3
    }

    property int tickPosition: (Slider.TickPosition.NoTicks)
    property int tickCount: ((to - from) / stepSize)

    function getSliderHandlePosition() {
        if (Slider.TickPosition.NoTicks === tickPosition) {
            return SliderHandle.ArrowPosition.NoArrow;
        }

        if (horizontal) {
            if (Slider.TickPosition.TicksAbove === tickPosition) {
                return SliderHandle.ArrowPosition.ArrowAbove;
            } else if (Slider.TickPosition.TicksBelow === tickPosition) {
                return SliderHandle.ArrowPosition.ArrowBelow;
            }
        } else {
            if (Slider.TickPosition.TicksLeft === tickPosition) {
                return SliderHandle.ArrowPosition.ArrowLeft;
            } else if (Slider.TickPosition.TicksRight === tickPosition) {
                return SliderHandle.ArrowPosition.ArrowRight;
            }
        }
    }

    implicitWidth: Math.max(background ? background.implicitWidth : 0,
                            (handle ? handle.implicitWidth : 0) + leftPadding + rightPadding)
    implicitHeight: Math.max(background ? background.implicitHeight : 0,
                             (handle ? handle.implicitHeight : 0) + topPadding + bottomPadding)

    // 绘制滑块（handle）
    handle: SliderHandle {
        id: sliderHandle
        x: control.leftPadding + (control.horizontal ? control.visualPosition * (control.availableWidth - width) : (control.availableWidth - width) / 2)
        y: control.topPadding + (control.horizontal ? (control.availableHeight - height) / 2 : control.visualPosition * (control.availableHeight - height))
        width: horizontal ? PM.Slider_Handle_Width : PM.Slider_Handle_Height
        height: horizontal ? PM.Slider_Handle_Height : PM.Slider_Handle_Width
        color: control.palette.highlight
        arrowPosition: getSliderHandlePosition()
    }

    // 绘制面板（panel）
    background: Rectangle {
        x: control.leftPadding + (control.horizontal ? 0 : (control.availableWidth - width) / 2)
        y: control.topPadding + (control.horizontal ? (control.availableHeight - height) / 2 : 0)
        implicitWidth: control.horizontal ? PM.Slider_Width : PM.Slider_Height
        implicitHeight: control.horizontal ? PM.Slider_Height : PM.Slider_Width
        scale: control.horizontal && control.mirrored ? -1 : 1

        // 绘制滑槽（groove）
        Rectangle {
            id: sliderGroove
            x: control.horizontal ? sliderHandle.width / 2 : (parent.width - width) / 2
            y: control.horizontal ? (parent.height - height) / 2 : sliderHandle.height / 2
            width: control.horizontal ? parent.width - sliderHandle.width : PM.Slider_Groove_height
            height: control.horizontal ? PM.Slider_Groove_height : parent.height - sliderHandle.height
            Shape {
                ShapePath {
                    capStyle: ShapePath.FlatCap
                    strokeStyle: ShapePath.DashLine
                    strokeColor: control.palette.button
                    strokeWidth: control.horizontal ? sliderGroove.height : sliderGroove.width
                    dashOffset: 0
                    dashPattern: [0.5, 0.25]
                    startX: control.horizontal ? 0 : sliderGroove.width / 2
                    startY: control.horizontal ? sliderGroove.height / 2 : 0
                    PathLine {
                        x: control.horizontal ? sliderGroove.width : sliderGroove.width / 2
                        y: control.horizontal ? sliderGroove.height / 2 : sliderGroove.height
                    }
                }
            }
        }

        // 绘制刻度线（tickmarks）
        Repeater {
            id: repeaterTop
            visible: false
            model: (Slider.TickPosition.TicksBothSides === tickPosition) ? (tickCount + 1) : 0
            width: horizontal ? sliderGroove.width : (parent.width - sliderHandle.width) / 2
            height: horizontal ? (parent.height - sliderHandle.height) / 2 : sliderGroove.height
            Rectangle {
                x: horizontal ? sliderGroove.x + index * (repeaterTop.width / (repeaterTop.count - 1)) : 0
                y: horizontal ? 0 : sliderGroove.y + index * (repeaterTop.height / (repeaterTop.count - 1))
                width: horizontal ? 1 : repeaterTop.width
                height: horizontal ? repeaterTop.height : 1
                color: control.palette.text
            }
        }
        Repeater {
            id: repeaterBottom
            visible: false
            model: (Slider.TickPosition.NoTicks === tickPosition) ? 0 : (tickCount + 1)
            width: repeaterTop.width
            height: repeaterTop.height
            Rectangle {
                x: horizontal ? sliderGroove.x + index * (repeaterBottom.width / (repeaterBottom.count - 1)) :
                                repeaterBottom.width + sliderHandle.width
                y: horizontal ? repeaterBottom.height + sliderHandle.height :
                                sliderGroove.y + index * (repeaterBottom.height / (repeaterBottom.count - 1))
                width: horizontal ? 1 : repeaterBottom.width
                height: horizontal ? repeaterBottom.height : 1
                color: control.palette.text
            }
        }
    }
}

