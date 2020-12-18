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

#ifndef DQUICKICONPROVIDER_H
#define DQUICKICONPROVIDER_H

#include <QQuickImageProvider>

#include <dtkdeclarative_global.h>

DQUICK_BEGIN_NAMESPACE

class DQuickIconProvider : public QQuickImageProvider
{
public:
    DQuickIconProvider();

protected:
    QPixmap requestPixmap(const QString &id, QSize *size, const QSize &requestedSize) override;
};

DQUICK_END_NAMESPACE

#endif // DQUICKICONPROVIDER_H
