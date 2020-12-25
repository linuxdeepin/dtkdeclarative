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
#include "dqmlglobalobject.h"
#include "dquickitemviewport.h"
#include "dquickiconprovider.h"
#include "dquickiconfinder.h"

#include <DFontManager>

#include <QQmlEngine>
#include <qqml.h>

DGUI_USE_NAMESPACE

DQUICK_BEGIN_NAMESPACE

void QmlpluginPlugin::registerTypes(const char *uri)
{
    qmlRegisterModule(uri, 1, 0);
    // @uri com.deepin.dtk
    qmlRegisterType<DQuickWindow>(uri, 1, 0, "DWindow");
    qmlRegisterType<DQuickIconFinder>(uri, 1, 0, "DIconFinder");
    // DIcon 是由　Image 和　IconFinder　封装成的一个QML类型
    qmlRegisterType(QUrl(QStringLiteral("qrc:/dtk/declarative/qml/DIcon.qml")), uri, 1, 0, "DIcon");

    //DQMLGlobalObject 依赖 DWindowManagerHelper中枚举的定义，所以需要先注册
    qmlRegisterType<DWindowManagerHelper>(uri, 1, 0, "DWindowManagerHelper");
    qmlRegisterSingletonType<DQMLGlobalObject>(uri, 1, 0, "DTK", [](QQmlEngine *, QJSEngine *) -> QObject * {
        return new DQMLGlobalObject;
    });
    qmlRegisterType<DQuickItemViewport>(uri, 1, 0, "DItemViewport");
    qmlRegisterType<DFontManager>(uri, 1, 0, "DFontManager");
}

void QmlpluginPlugin::initializeEngine(QQmlEngine *engine, const char *uri)
{
    engine->addImageProvider("dtk.icon", new DQuickIconProvider);
    QQmlExtensionPlugin::initializeEngine(engine, uri);
}

DQUICK_END_NAMESPACE
