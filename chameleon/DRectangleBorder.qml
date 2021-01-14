/*
 * Copyright (C) 2020 ~ 2020 Uniontech Technology Co., Ltd.
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
import "PixelMetric.js" as PM

Rectangle {
    anchors.centerIn: parent
    width: parent.width + 2 * (PM.ControlFocusBorderWidth + PM.ControlFocusSpaceWidth)
    height: parent.height + 2 * (PM.ControlFocusBorderWidth + PM.ControlFocusSpaceWidth)
    radius: PM.ControlRadius + PM.ControlFocusBorderWidth + PM.ControlFocusSpaceWidth
    border { width: PM.ControlFocusBorderWidth; color: "transparent" }
    color: "transparent"
    visible: false
}
