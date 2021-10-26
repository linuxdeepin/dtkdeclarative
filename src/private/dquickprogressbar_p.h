/*
 * Copyright (C) 2020 ~ 2021 Uniontech Software Technology Co.,Ltd.
 *
 * Author:     xiaoyaobing <xiaoyaobing@uniontech.com>
 *
 * Maintainer: xiaoyaobing <xiaoyaobing@uniontech.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef DQUICKPROGRESSBAR_P_H
#define DQUICKPROGRESSBAR_P_H

#include "dquickprogressbar.h"

#include <private/qquickitem_p.h>

class DQuickProgressBarPrivate : public QQuickItemPrivate
{
    Q_DECLARE_PUBLIC(DQuickProgressBar)

public:
    DQuickProgressBarPrivate()
        : radius(0)
        , value(0)
        , orientation(Qt::Vertical)
    {

    }

public:
    qreal radius;  // 圆角大小
    qreal value;   // range 0-1
    QColor color;  // 渲染颜色,默认白色
    Qt::Orientation orientation; // 默认竖直
};

#endif // DQUICKPROGRESSBAR_P_H
