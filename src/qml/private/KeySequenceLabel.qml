/*
 * Copyright (C) 2022 UnionTech Technology Co., Ltd.
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
import org.deepin.dtk 1.0 as D
import org.deepin.dtk.style 1.0 as DS
import ".."

Control {
    id: control
    property string text
    property D.Palette textColor: DS.Style.keySequenceEdit.labelTextColor
    property D.Palette backgroundColor1: DS.Style.keySequenceEdit.labelBackgroundColor1
    property D.Palette backgroundColor2: DS.Style.keySequenceEdit.labelBackgroundColor2
    property D.Palette shadowInnerColor1: DS.Style.keySequenceEdit.labelShadowInner1
    property D.Palette shadowInnerColor2: DS.Style.keySequenceEdit.labelShadowInner2
    property D.Palette shadowOuterColor: DS.Style.keySequenceEdit.labelShadowOuter

    leftPadding: DS.Style.keySequenceEdit.labelLeftRightPadding
    topPadding: DS.Style.keySequenceEdit.labelTopbottomPadding
    rightPadding: leftPadding
    bottomPadding: topPadding

    contentItem: Text {
        text: control.text
        horizontalAlignment: Qt.AlignHCenter
        verticalAlignment: Qt.AlignVCenter
        font: D.DTK.fontManager.t8
        color: control.D.ColorSelector.textColor
    }

    background: Item {
        BoxShadow {
            anchors.fill: parent
            cornerRadius: backgroundRect.radius
            shadowOffsetY: 1
            shadowBlur: 2
            shadowColor: control.D.ColorSelector.shadowOuterColor;
            hollow: true
        }

        Rectangle {
            id: backgroundRect
            anchors.fill: parent

            radius: DS.Style.control.radius
            gradient: Gradient {
                GradientStop {
                    position: 0.0
                    color: control.D.ColorSelector.backgroundColor1
                }
                GradientStop {
                    position: 0.98
                    color: control.D.ColorSelector.backgroundColor2
                }
            }
        }

        BoxInsetShadow {
            anchors.fill: parent
            cornerRadius: backgroundRect.radius
            shadowOffsetY: -2
            shadowBlur: 1
            shadowColor: control.D.ColorSelector.shadowInnerColor1
        }

        BoxInsetShadow {
            visible: D.DTK.themeType === D.ApplicationHelper.DarkType
            anchors.fill: parent
            cornerRadius: backgroundRect.radius
            shadowOffsetY: 1
            shadowBlur: 1
            shadowColor: control.D.ColorSelector.shadowInnerColor2
        }
    }
}
