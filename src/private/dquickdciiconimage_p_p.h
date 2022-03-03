/*
 * Copyright (C) 2022 UnionTech Technology Co., Ltd.
 *
 * Author:     Chen Bin <chenbin@uniontech.com>
 *
 * Maintainer: Chen Bin <chenbin@uniontech.com>
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
#ifndef DQUICKDCIICONIMAGE_P_P_H
#define DQUICKDCIICONIMAGE_P_P_H

#include "dquickiconimage_p.h"
#include "dqmlglobalobject_p.h"
#include "dquickdciiconimage_p.h"
#include "dquickiconimage_p_p.h"

#include <dobject_p.h>
#include <DDciIconPalette>

DQUICK_BEGIN_NAMESPACE
class DQuickDciIconImageItemPrivate;
class DQuickDciIconImageItemPrivate : public DQuickIconImagePrivate
{
    Q_DECLARE_PUBLIC(DQuickIconImage)

public:
    DQuickDciIconImageItemPrivate(DQuickDciIconImagePrivate *pqq);
    void maybeUpdateUrl();
    QUrlQuery getUrlQuery();

private:
    DQuickDciIconImagePrivate *parentPriv;
};

class DQuickDciIconImagePrivate : public DCORE_NAMESPACE::DObjectPrivate
{
    Q_DECLARE_PUBLIC(DQuickDciIconImage)

public:
    DQuickDciIconImagePrivate(DQuickDciIconImage *qq);
    void layout();
    void updateImageSourceUrl();

    QString name;
    DDciIconPalette palette;
    DQuickIconImage *imageItem;
    DQMLGlobalObject::ControlState mode = DQMLGlobalObject::NormalState;
    DGuiApplicationHelper::ColorType theme = DGuiApplicationHelper::ColorType::LightType;
    bool fallbackToQIcon = true;
};

DQUICK_END_NAMESPACE
#endif // DQUICKDCIICONIMAGE_P_P_H
