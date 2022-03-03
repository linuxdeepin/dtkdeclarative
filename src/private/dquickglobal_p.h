/*
 * Copyright (C) 2022 UnionTech Technology Co., Ltd.
 *
 * Author:     JiDe Zhang <zhangjide@deepin.org>
 *
 * Maintainer: JiDe Zhang <zhangjide@deepin.org>
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
#ifndef DQUICKGLOBAL_P_H
#define DQUICKGLOBAL_P_H

#include <QtGlobal>

#if QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)
#define ThrowError(obj, message) {\
    auto e = qmlEngine(obj);\
    if (e) e->throwError(message);\
    else qCritical().noquote() << message.toLocal8Bit();\
}
#else
#define ThrowError(obj, message) \
    qCritical().noquote() << message.toLocal8Bit()
#endif

#endif // DQUICKGLOBAL_P_H
