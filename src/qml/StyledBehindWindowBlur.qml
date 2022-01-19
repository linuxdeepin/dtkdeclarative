/*
 * Copyright (C) 2022 UnionTech Technology Co., Ltd.
 *
 * Author:     JiDe Zhang <zhangjide@deepin.org>
 *
 * Maintainer: JiDe Zhang <zhangjide@deepin.org>
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

import QtQuick.Controls 2.15
import org.deepin.dtk.impl 1.0 as D
import org.deepin.dtk.style 1.0 as DS

D.BehindWindowBlur {
    // maybe is a ApplicationWindow or Control
    property var control

    blendColor: {
        if (valid) {
            return D.DTK.selectColor(control.palette.window,
                                     DS.Style.behindWindowBlur.lightColor,
                                     DS.Style.behindWindowBlur.darkColor)
        }
        return D.DTK.selectColor(control.palette.window,
                                 DS.Style.behindWindowBlur.lightNoBlurColor,
                                 DS.Style.behindWindowBlur.darkNoBlurColor)
    }
}
