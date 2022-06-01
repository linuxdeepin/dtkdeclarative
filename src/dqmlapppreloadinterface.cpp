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

#include "dqmlapppreloadinterface.h"

#include <QGuiApplication>

DQUICK_BEGIN_NAMESPACE


void DQmlAppPreloadInterface::aboutToPreload(QQmlApplicationEngine *engine) {
    Q_UNUSED(engine);
}

QGuiApplication *DQmlAppPreloadInterface::creatApplication(int &argc, char **argv) {
    if (qGuiApp)
        return qGuiApp;
    return new QGuiApplication(argc, argv);
}

QSGRendererInterface::GraphicsApi DQmlAppPreloadInterface::graphicsApi() {
    return QSGRendererInterface::Unknown;  // auto select.
}

DQUICK_END_NAMESPACE
