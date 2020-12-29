/*
 * Copyright (C) 2020 ~ 2020 Uniontech Technology Co., Ltd.
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

#ifndef DQUICKPALETTE_P_H
#define DQUICKPALETTE_P_H

#include "dquickpalette.h"

#include <QPointer>
#include <DPalette>
#include <DObjectPrivate>

DGUI_USE_NAMESPACE
DQUICK_BEGIN_NAMESPACE

class DQuickPalettePrivate : public DTK_CORE_NAMESPACE::DObjectPrivate
{
public:
    DQuickPalettePrivate(DQuickPalette *qq);

    DPalette palette;
    // 初始化为true是为了能正常在c++代码中使用
    // 因为classBegin和componentComplete只会
    // 在qml中使用时才会被调用，因此当classBegin
    // 未被调用时，有理由认为其是已初始化的
    bool componentComplete = true;
    QPointer<QQuickItem> item;
    QPointer<QQuickWindow> itemWindow;

    void initSourceItem(QQuickItem *old, QQuickItem *item);
    void _q_updatePalette();
    void _q_onWindowChanged(QQuickWindow *window);

private:
    D_DECLARE_PUBLIC(DQuickPalette)
};

DQUICK_END_NAMESPACE

#endif // DQUICKPALETTE_P_H
