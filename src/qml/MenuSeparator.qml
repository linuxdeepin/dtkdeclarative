/*
 * Copyright (C) 2022 UnionTech Technology Co., Ltd.
 *
 * Author:     yeshanshan <yeshanshan@uniontech.com>
 *
 * Maintainer: yeshanshan <yeshanshan@uniontech.com>
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
import QtQuick.Templates 2.4 as T
import org.deepin.dtk.impl 1.0 as D
import org.deepin.dtk.style 1.0 as DS

T.MenuSeparator {
    id: control

    property string text
    property bool __lineStyle: control.text === ""

    implicitWidth: Math.max(background ? background.implicitWidth : 0, contentItem.implicitWidth + leftPadding + rightPadding)
    implicitHeight: Math.max(background ? background.implicitHeight : 0, contentItem.implicitHeight + topPadding + bottomPadding)

    topPadding: __lineStyle ? DS.Style.menu.separatorLineTopPadding : DS.Style.menu.separatorTopPadding
    bottomPadding: __lineStyle ? DS.Style.menu.separatorLineBottomPadding : DS.Style.menu.separatorBottomPadding

    contentItem: Loader {
        sourceComponent: __lineStyle ? lineCom : titleCom

        Component {
            id: lineCom
            Rectangle {
                width: control.parent.width
                implicitHeight: DS.Style.menu.DS.Style.menu.separatorLineHeight
                color: DS.Style.menu.separatorLineColor
            }
        }

        Component {
            id: titleCom
            Text {
                text: control.text
                opacity: 0.5
                font: D.DTK.fontManager.t8
            }
        }
    }
}
