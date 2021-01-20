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

#ifndef DQMLAPPPLUGININTERFACE_H
#define DQMLAPPPLUGININTERFACE_H

#include <QGuiApplication>

#include <dtkdeclarative_global.h>

#define DQmlAppPluginInterface_iid "dtk.qml.app.plugin.interface"

QT_BEGIN_NAMESPACE
class QQmlApplicationEngine;
QT_END_NAMESPACE

DQUICK_BEGIN_NAMESPACE

class DQmlAppPluginInterface
{
public:
    DQmlAppPluginInterface() = default;
    virtual ~DQmlAppPluginInterface() = default;
    virtual int main(QGuiApplication *app, QQmlApplicationEngine *engine) = 0;
    virtual QGuiApplication *createApplication(int &argc, char **argv) {
        return new QGuiApplication(argc, argv);
    }
};

DQUICK_END_NAMESPACE

Q_DECLARE_INTERFACE(DTK_QUICK_NAMESPACE::DQmlAppPluginInterface, DQmlAppPluginInterface_iid)
#endif // DQMLAPPPLUGININTERFACE_H
