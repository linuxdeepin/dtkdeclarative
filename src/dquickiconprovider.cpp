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

#include <QIcon>
#include <QUrlQuery>

#include "dquickiconprovider.h"
#include "dquickiconfinder.h"

DQUICK_BEGIN_NAMESPACE

DQuickIconProvider::DQuickIconProvider()
    : QQuickImageProvider(QQuickImageProvider::Pixmap)
{

}

QPixmap DQuickIconProvider::requestPixmap(const QString &id, QSize *size, const QSize &requestedSize)
{
    Q_UNUSED(requestedSize);
    Q_UNUSED(size);

    auto obj = DQuickIconFinder::queryToObject(id);
    if (!obj) {
        return QPixmap();
    }

    return obj->pixmap();
}

DQUICK_END_NAMESPACE
