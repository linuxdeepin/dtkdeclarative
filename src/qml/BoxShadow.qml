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

Item {
    property alias shadowBlur: shadow.shadowBlur
    property alias shadowColor: shadow.shadowColor
    property real shadowOffsetX
    property real shadowOffsetY
    property alias inner: shadow.isInner
    property alias cornerRadius: shadow.cornerRadius
    property alias cache: shadow.cache
    property alias spread: shadow.spread

    D.ShadowImage {
        id: shadow

        x: isInner ? 0 : -parent.shadowBlur + parent.shadowOffsetX
        y: isInner ? 0 : -parent.shadowBlur + parent.shadowOffsetY
        width: isInner ? parent.width : parent.width + parent.shadowBlur * 2
        height: isInner ? parent.height : parent.height + parent.shadowBlur * 2
    }
}
