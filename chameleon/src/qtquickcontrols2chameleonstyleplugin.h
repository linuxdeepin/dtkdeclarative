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

#include <QQuickItem>

class QtQuickControls2ChameleonStylePlugin : public QQuickItem
{
    Q_OBJECT
    Q_DISABLE_COPY(QtQuickControls2ChameleonStylePlugin)

public:
    QtQuickControls2ChameleonStylePlugin(QQuickItem *parent = nullptr);
    ~QtQuickControls2ChameleonStylePlugin();
};

#endif // QTQUICKCONTROLS2CHAMELEONSTYLEPLUGIN_H
