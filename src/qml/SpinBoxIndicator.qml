/*
 * Copyright (C) 2021 ~ 2022 UnionTech Technology Co., Ltd.
 *
 * Author:     Chen Bin <chenbin@uniontech.com>
 *
 * Maintainer: Chen Bin <chenbin@uniontech.com>
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

import QtQuick 2.11
import org.deepin.dtk.impl 1.0 as D
import org.deepin.dtk.style 1.0 as DS

Control {
    id: control
    enum IndicatorDirection {
        UpIndicator = 0,
        DownIndicator = 1
    }

    property Item spinBox
    property bool pressed
    property bool singleIndicator: false
    property int direction
    property D.Palette inactiveBackgroundColor: DS.Style.spinBoxIndicatorBackground

    palette.windowText: control.D.ColorSelector.inactiveBackgroundColor
    hoverEnabled: true
    implicitWidth: DS.Style.spinBox.indicatorWidth
    implicitHeight: spinBox.activeFocus ? spinBox.implicitHeight / 2 : DS.Style.spinBox.indicatorHeight
    opacity: D.ColorSelector.controlState === D.DTK.DisabledState ? 0.4 : 1

    Component {
        id: inactiveComponent
        D.DciIcon {
            id: icon
            sourceSize.width: DS.Style.spinBox.indicatorIconSize
            palette: D.DTK.makeIconPalette(control.palette)
            name: direction === SpinBoxIndicator.IndicatorDirection.UpIndicator ? "entry_spinbox_up" : "entry_spinbox_down"
            mode: control.D.ColorSelector.controlState
            theme: control.D.ColorSelector.controlTheme
            fallbackToQIcon: false
        }
    }

    Component {
        id: activeComponent
        Item {
            Item {
                id: btnBackground
                width: parent.width + DS.Style.control.radius
                height: parent.height + DS.Style.control.radius
                anchors {
                    right: parent.right
                    top: direction === SpinBoxIndicator.IndicatorDirection.UpIndicator ? parent.top : undefined
                    bottom: direction === SpinBoxIndicator.IndicatorDirection.UpIndicator ? undefined : parent.bottom
                }

                Button {
                    id: btn
                    anchors {
                        right: parent.right
                        top: direction === SpinBoxIndicator.IndicatorDirection.UpIndicator ? parent.top : undefined
                        bottom: direction === SpinBoxIndicator.IndicatorDirection.UpIndicator ? undefined : parent.bottom
                    }
                    width: DS.Style.spinBox.indicatorWidth
                    height: spinBox.implicitHeight / 2
                    icon.name: direction === SpinBoxIndicator.IndicatorDirection.UpIndicator ? "go-up" : "go-down"
                    activeFocusOnTab: false
                    opacity: 1
                    background: ButtonPanel {
                        button: btn
                        radius: 0
                    }
                    onClicked: {
                        if (direction === SpinBoxIndicator.IndicatorDirection.UpIndicator) {
                            spinBox.increase()
                        } else {
                            spinBox.decrease()
                        }
                    }
                }
            }

            D.ItemViewport {
                id: viewport
                sourceItem: btnBackground
                radius: DS.Style.control.radius
                fixed: true
                width: btnBackground.width
                height: btnBackground.height
                anchors {
                    right: parent.right
                    top: direction === SpinBoxIndicator.IndicatorDirection.UpIndicator ? parent.top : undefined
                    bottom: direction === SpinBoxIndicator.IndicatorDirection.UpIndicator ? undefined : parent.bottom
                }
                hideSource: true
            }
        }

    }

    Loader {
        anchors.fill: parent
        sourceComponent: spinBox.activeFocus ? activeComponent : inactiveComponent
    }
}
