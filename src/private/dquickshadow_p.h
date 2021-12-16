/*
 * Copyright (C) 2021 UnionTech Technology Co., Ltd.
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

#ifndef DQUICKSHADOW_P_H
#define DQUICKSHADOW_P_H

#include "dquickshadow.h"

#include <private/qquickitem_p.h>

DQUICK_BEGIN_NAMESPACE

class DQuickShadowPrivate : public QQuickItemPrivate
{
    Q_DECLARE_PUBLIC(DQuickShadow)

public:
    DQuickShadowPrivate()
        : QQuickItemPrivate()
        , fill(false)
    {

    }

    qreal shadowRadius;  // 阴影半径
    QColor shadowColor;  // 阴影颜色
    qreal spread;        // 扩展系数
    qreal relativeSizeX;
    qreal relativeSizeY;
    bool fill;
};

DQUICK_END_NAMESPACE

#endif // DQUICKSHADOW_P_H
