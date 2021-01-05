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
#ifndef QTQUICKCONTROLS2CHAMELEONSTYLEPLUGIN_H
#define QTQUICKCONTROLS2CHAMELEONSTYLEPLUGIN_H

#ifdef USE_QQuickStylePlugin
#include <private/qquickstyleplugin_p.h>
#else
#include <QQmlExtensionPlugin>
#endif

class QtQuickControls2ChameleonStylePlugin
#ifdef USE_QQuickStylePlugin
        : public QQuickStylePlugin
#else
        : public QQmlExtensionPlugin
#endif
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID QQmlExtensionInterface_iid)

public:
    ~QtQuickControls2ChameleonStylePlugin() override;
#ifdef USE_QQuickStylePlugin
    QString name() const override;
#endif
    void registerTypes(const char *uri) override;
};

#endif // QTQUICKCONTROLS2CHAMELEONSTYLEPLUGIN_H
