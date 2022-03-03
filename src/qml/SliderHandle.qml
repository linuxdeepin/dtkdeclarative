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

import QtQuick 2.11
import org.deepin.dtk.impl 1.0 as D

D.DciIcon {
    id: control

    property int type: Slider.HandleType.NoArrowHorizontal

    function getIconNameByType(handleType) {
        switch(handleType) {
        case Slider.HandleType.NoArrowHorizontal:
            return "slider_round_hor"
        case Slider.HandleType.NoArrowVertical:
            return "slider_round_ver"
        case Slider.HandleType.ArrowUp:
            return "slider_point_up"
        case Slider.HandleType.ArrowBottom:
            return "slider_point_down"
        case Slider.HandleType.ArrowLeft:
            return "slider_point_left"
        case Slider.HandleType.ArrowRight:
            return "slider_point_left"
        }
    }

    sourceSize.width: control.width
    sourceSize.height: control.height
    name: getIconNameByType(type)
    mirror: Slider.HandleType.ArrowRight === control.type
    fallbackToQIcon: false
}
