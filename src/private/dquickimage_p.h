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

#ifndef DQUICKIMAGE_P_H
#define DQUICKIMAGE_P_H

#include "dquickimage.h"

#include <private/qquickimage_p_p.h>

class DQuickImagePrivate : public QQuickImagePrivate
{
    Q_DECLARE_PUBLIC(DQuickImage)
public:
    DQuickImagePrivate()
        : QQuickImagePrivate()
        , renderType(DQuickImage::AutoRender)
        , radius(0)
    {

    }
public:
    DQuickImage::RenderType renderType;         // 渲染模式区分是指定颜色渲染还是自动渲染原图片
    QColor specifyColor;                        // 指定渲染的颜色
    qreal radius;                               // 圆角半径
};

#endif // DQUICKIMAGE_P_H
