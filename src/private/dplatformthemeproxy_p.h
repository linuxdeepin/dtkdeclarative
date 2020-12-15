/*
 * Copyright (C) 2020 ~ 2020 Deepin Technology Co., Ltd.
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

#ifndef DPLATFORMTHEMEPROXY_P_H
#define DPLATFORMTHEMEPROXY_P_H

#include <DObjectPrivate>
#include <DObject>
#include <DPlatformTheme>

#include "dplatformthemeproxy.h"

DQUICK_BEGIN_NAMESPACE

class DPlatformThemeProxyPrivate : public DTK_CORE_NAMESPACE::DObjectPrivate
{
public:
    DPlatformThemeProxyPrivate(DPlatformThemeProxy *qq);

    DPlatformTheme *proxy;

private:
    D_DECLARE_PUBLIC(DPlatformThemeProxy)
};

DQUICK_END_NAMESPACE

#endif // DPLATFORMTHEMEPROXY_P_H
