/*
 * Copyright (C) 2020 ~ 2020 Deepin Technology Co., Ltd.
 *
 * Author:     sunkang <sunkang@uniontech.com>
 *
 * Maintainer: sunkang <sunkang@uniontech.com>
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

#include "qmlplugin_plugin.h"

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickStyle>

DQUICK_USE_NAMESPACE

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);

    QGuiApplication app(argc, argv);

    QQuickStyle::setStyle("Chameleon");

    //这里为方便调试，直接使用这种方式注册，实际使用时直接安装对应的包即可使用 import com.deepin.dtk 1.0
    // 为防止冲突 这里使用 com.deepin.demo
    QmlpluginPlugin plugin;
    plugin.registerTypes("com.deepin.demo");

    QQmlApplicationEngine engine;
    plugin.initializeEngine(&engine, nullptr);
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
