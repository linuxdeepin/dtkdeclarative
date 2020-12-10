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
#include <QPluginLoader>
#include <QScopedPointer>

#include "dapploader.h"
#include "dqmlappplugininterface.h"
#include "private/dapploader_p.h"

DQUICK_BEGIN_NAMESPACE

DAppLoaderPrivate::DAppLoaderPrivate(DAppLoader *qq)
    : DTK_CORE_NAMESPACE::DObjectPrivate(qq)
{
    qmlPlugin = QString();
}

/*!
 * \~chinese \brief DAppLoader::DAppLoader　用于加载DTk QML应用插件
 * \~chinese \param appName　　应用插件的名字
 * \~chinese \param appPath　　应用插件的安装目录
 */
DAppLoader::DAppLoader(const QString &appName, const QString &appPath)
    : DTK_CORE_NAMESPACE::DObject(*new DAppLoaderPrivate(this))
{
    D_D(DAppLoader);
    QString path = appPath;

#ifdef DTK_QML_APP_PLUGIN_PATH
    if (path.isEmpty()) {
        path = DTK_QML_APP_PLUGIN_PATH;
    }
#endif

    d->qmlPlugin = path + "/libdtkqml-" + appName + ".so";
}

/*!
 * \~chinese \brief DAppLoader::exec　解析DTK QML应用插件
 * \~chinese \param app　　应用对应的QGuiApplication
 * \~chinese \param engine　　应用对应的QQmlApplicationEngine
 */
int DAppLoader::exec(QGuiApplication *app, QQmlApplicationEngine *engine)
{
    D_D(DAppLoader);
    QPluginLoader loader(d->qmlPlugin);
    QScopedPointer<QObject> instant(loader.instance());
    if (!instant.isNull()) {
        DQmlAppPluginInterface *interface = qobject_cast<DQmlAppPluginInterface *>(instant.data());
        return interface->main(app, engine);
    }
    return 0;
}

DQUICK_END_NAMESPACE
