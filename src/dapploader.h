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
#ifndef DAPPLOADER_H
#define DAPPLOADER_H

#include <QString>
#include <DObject>
#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include <dtkdeclarative_global.h>

DQUICK_BEGIN_NAMESPACE

class DAppLoaderPrivate;
class DAppLoader : public DTK_CORE_NAMESPACE::DObject
{
public:
    DAppLoader() = delete ;
    DAppLoader(const QString &appName, const QString &appPath = QString());
    ~DAppLoader() = default;
    int exec(QGuiApplication *app, QQmlApplicationEngine *engine);
private:
    D_DECLARE_PRIVATE(DAppLoader)
};

DQUICK_END_NAMESPACE

#endif // DAPPLOADER_H
