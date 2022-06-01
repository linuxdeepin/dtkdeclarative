/*
 * Copyright (C) 2022 UnionTech Technology Co., Ltd.
 *
 * Author:     Chen Bin <chenbin@uniontech.com>
 *
 * Maintainer: Chen Bin <chenbin@uniontech.com>
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
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef DQMLAPPMAINWINDOWINTERFACE_H
#define DQMLAPPMAINWINDOWINTERFACE_H

#include <QGuiApplication>

#include <dtkdeclarative_global.h>

#define DQmlAppMainWindowInterface_iid "dtk.qml.app.mainwindow.interface"

QT_BEGIN_NAMESPACE
class QQmlApplicationEngine;
QT_END_NAMESPACE

DQUICK_BEGIN_NAMESPACE

class DQmlAppMainWindowInterface
{
public:
    DQmlAppMainWindowInterface() = default;
    virtual ~DQmlAppMainWindowInterface() = default;

    virtual QUrl mainComponentPath() const = 0;
    virtual void initialize(QQmlApplicationEngine *engine);
    virtual void finishedLoading(QQmlApplicationEngine *engine);
};

DQUICK_END_NAMESPACE

Q_DECLARE_INTERFACE(DTK_QUICK_NAMESPACE::DQmlAppMainWindowInterface, DQmlAppMainWindowInterface_iid)
#endif // DQMLAPPPLUGININTERFACE_H
