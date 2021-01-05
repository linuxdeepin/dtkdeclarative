/*
 * Copyright (C) 2020 Uniontech Technology Co., Ltd.
 *
 * Author:     zccrs <zccrs@live.com>
 *
 * Maintainer: zccrs <zhangjide@uniontech.com>
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

#ifndef DQUICKPALETTEPROVIDER_P_H
#define DQUICKPALETTEPROVIDER_P_H

#include <dtkdeclarative_global.h>
#include <private/qqmlglobal_p.h>

DQUICK_BEGIN_NAMESPACE

class DQuickPaletteProvider : public QQmlValueTypeProvider
{
public:
    static void init();
    static void cleanup();

    const QMetaObject *getMetaObjectForMetaType(int type) override;
    bool init(int type, QVariant& dst) override;
    bool equal(int type, const void *lhs, const QVariant &rhs) override;
    bool store(int type, const void *src, void *dst, size_t dstSize) override;
    bool read(const QVariant &src, void *dst, int dstType) override;
    bool write(int type, const void *src, QVariant& dst) override;
};

DQUICK_END_NAMESPACE

#endif // DQUICKPALETTEPROVIDER_P_H
