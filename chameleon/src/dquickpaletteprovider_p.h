// SPDX-FileCopyrightText: 2020 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

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
