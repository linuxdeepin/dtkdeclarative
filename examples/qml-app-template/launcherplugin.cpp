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

#include <QQmlContext>

#include "launcherplugin.h"

LauncherPlugin::LauncherPlugin(QObject *parent)
    : QObject(parent)
{

}

LauncherPlugin::~LauncherPlugin()
{

}

int LauncherPlugin::main(QGuiApplication *app, QQmlApplicationEngine *engine)
{
    // 请在此处注册需要导入到QML中的C++类型
    // 例如： engine->rootContext()->setContextProperty("Utils", new Utils);
    engine->load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine->rootObjects().isEmpty())
        return -1;

    return app->exec();
}
