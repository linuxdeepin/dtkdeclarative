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

#ifndef DQUICKOPACITYMASK_P_P_H
#define DQUICKOPACITYMASK_P_P_H

#include "dquickopacitymask_p.h"

#include <private/qquickitem_p.h>

DQUICK_BEGIN_NAMESPACE

class DQuickOpacityMaskPrivate : public QQuickItemPrivate
{
    Q_DECLARE_PUBLIC(DQuickOpacityMask)

public:
    DQuickOpacityMaskPrivate()
        : source(nullptr)
        , maskSource(nullptr)
        , invert(false)
        , sourceChanged(false)
    {}

    QQuickItem *source;
    QQuickItem *maskSource;
    bool invert;
    bool sourceChanged;
};

DQUICK_END_NAMESPACE

#endif // DQUICKOPACITYMASK_P_P_H
