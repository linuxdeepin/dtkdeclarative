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

#ifndef DQUICKGLOW_P_P_H
#define DQUICKGLOW_P_P_H

#include "dquickglow_p.h"

#include <private/qquickitem_p.h>

DQUICK_BEGIN_NAMESPACE

class DQuickGlowPrivate : public QQuickItemPrivate
{
    Q_DECLARE_PUBLIC(DQuickGlow)

public:
    DQuickGlowPrivate()
        : QQuickItemPrivate()
        , fill(false)
    {

    }

    qreal glowRadius;
    QColor color;
    qreal spread;
    qreal relativeSizeX;
    qreal relativeSizeY;
    bool fill;
};

DQUICK_END_NAMESPACE

#endif // DQUICKGLOW_P_P_H
