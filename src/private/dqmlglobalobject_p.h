/*
 * Copyright (C) 2020 ~ 2021 deepin Technology Co., Ltd.
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

#ifndef DQMLGLOBALOBJECT_P_H
#define DQMLGLOBALOBJECT_P_H

#include <DObjectPrivate>
#include <DObject>
#include <DWindowManagerHelper>
#include <DPlatformThemeProxy>

#include "dqmlglobalobject.h"

DQUICK_BEGIN_NAMESPACE

class DQMLGlobalObjectPrivate : public DTK_CORE_NAMESPACE::DObjectPrivate
{
public:
    DQMLGlobalObjectPrivate(DQMLGlobalObject *qq);

    void ensurePalette();
    void updatePalettes();
    void _q_onPaletteChanged();
    void ensureWebsiteInfo();

    mutable DPlatformThemeProxy *platformTheme = nullptr;

    bool paletteInit = false;
    QPalette palette;
    QPalette inactivePalette;
    QString deepinWebsiteName;
    QString deepinWebsiteLink;
    QString deepinDistributionOrgLogo;

private:
    D_DECLARE_PUBLIC(DQMLGlobalObject)
};

DQUICK_END_NAMESPACE

#endif // DQMLGLOBALOBJECT_P_H
