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

    property D.Palette indicatorBackgroundColor: DS.Style.spinBoxIndicatorBackground
    property D.Palette indicatorColor: DS.Style.spinBoxIndicator
    property int direction: IndicatorDirection.UpIndicator
    property bool singleIndicator: false
    property bool pressed
    property alias iconName: icon.name

    palette.windowText: D.ColorSelector.indicatorColor
    implicitWidth: DS.Style.spinBox.indicatorWidth
    implicitHeight: singleIndicator ? DS.Style.spinBox.indicatorHeight : DS.Style.spinBox.indicatorWidth
    hoverEnabled: true

    Item {
        anchors.left: parent.left
        anchors.bottom: (direction === SpinBoxIndicator.IndicatorDirection.UpIndicator) ? undefined : parent.bottom
        anchors.top: (direction === SpinBoxIndicator.IndicatorDirection.UpIndicator) ? parent.top : undefined
        implicitWidth: DS.Style.spinBox.indicatorWidth
        implicitHeight: singleIndicator ? DS.Style.spinBox.indicatorWidth / 2 : DS.Style.spinBox.indicatorWidth
        clip: singleIndicator

        Rectangle {
            anchors.left: parent.left
            anchors.bottom: (direction === SpinBoxIndicator.IndicatorDirection.UpIndicator) ? undefined : parent.bottom
            anchors.top: (direction === SpinBoxIndicator.IndicatorDirection.UpIndicator) ? parent.top : undefined
            implicitWidth: DS.Style.spinBox.indicatorWidth
            implicitHeight: DS.Style.spinBox.indicatorWidth
            radius: parent.width / 2
            color: control.D.ColorSelector.indicatorBackgroundColor
        }
    }

    Rectangle {
        visible: singleIndicator
        anchors.left: parent.left
        anchors.bottom: (direction === SpinBoxIndicator.IndicatorDirection.UpIndicator) ? parent.bottom : undefined
        anchors.top: (direction === SpinBoxIndicator.IndicatorDirection.UpIndicator) ? undefined : parent.top
        implicitWidth: parent.implicitWidth
        implicitHeight: DS.Style.spinBox.indicatorHeight - DS.Style.spinBox.indicatorWidth / 2
        color: control.D.ColorSelector.indicatorBackgroundColor
    }

    D.DciIcon {
        id: icon
        anchors.centerIn: parent
        sourceSize.width: DS.Style.spinBox.indicatorIconSize
        palette: D.DTK.makeIconPalette(control.palette)
    }
}
