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

#include "qmlplugin_plugin.h"
#include "dquickwindow.h"

#include <qqml.h>

DQUICK_BEGIN_NAMESPACE

void QmlpluginPlugin::registerTypes(const char *uri)
{
    // @uri com.deepin.dtk
    qmlRegisterType<DQuickWindow>(uri, 1, 0, "DWindow");
}

DQUICK_END_NAMESPACE
