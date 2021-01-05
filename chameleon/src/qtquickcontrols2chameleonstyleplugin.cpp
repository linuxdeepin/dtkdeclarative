/*
 * Copyright (C) 2020 ~ 2020 Deepin Technology Co., Ltd.
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
#include "qtquickcontrols2chameleonstyleplugin.h"
#include "dquickpaletteprovider_p.h"

#include <qqml.h>

DQUICK_USE_NAMESPACE

QtQuickControls2ChameleonStylePlugin::~QtQuickControls2ChameleonStylePlugin()
{
    // 恢复到使用QQuickPaletteProvider
    DQuickPaletteProvider::cleanup();
}

#ifdef USE_QQuickStylePlugin
QString QtQuickControls2ChameleonStylePlugin::name() const
{
    return QStringLiteral("Chameleon");
}
#endif

void QtQuickControls2ChameleonStylePlugin::registerTypes(const char *uri)
{
    // 为 DPalette/QPalette 类型注册QQmlValueTypeProvider
    DQuickPaletteProvider::init();

    qmlRegisterModule(uri, 2, 0);
}
