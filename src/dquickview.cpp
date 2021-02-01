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

#include "dquickview.h"
#include "private/dquickview_p.h"
#include <DQuickWindow>

DQUICK_BEGIN_NAMESPACE

DQuickViewPrivate::DQuickViewPrivate(DQuickView *qq)
    : DTK_CORE_NAMESPACE::DObjectPrivate(qq)
    , attached(new DQuickWindowAttached(qq))
{
}

/*!
 * \~chinese \brief DQuickView::DQuickView 模糊视图类，当给定qml主源文件的URL时，
 * \~chinese 自动加载qml和显示qml场景，窗口背景模糊
 * \~chinese 使用示例：
 * \~chinese DQuickView view;
 * \~chinese view.setColor("transparent");
 * \~chinese view.attached()->setEnableBlurWindow(true);
 * \~chinese   ....     // 使用QQuickView::setSource(const QUrl &url)加载qml文件
 * \~chinese view.show();
 * \~chinese \param parent
 */
DQuickView::DQuickView(QWindow *parent)
    : QQuickView(parent)
    , DObject(*new DQuickViewPrivate(this))
{
}

DQuickView::~DQuickView()
{
}

DQuickWindowAttached *DQuickView::attached() const
{
    D_DC(DQuickView);

    return d->attached;
}

DQuickWindowAttached *DQuickView::qmlAttachedProperties(QObject *object)
{
    QQuickView *view = qobject_cast<QQuickView *>(object);
    if (view) {
        return new DQuickWindowAttached(view);
    }

    return nullptr;
}

DQUICK_END_NAMESPACE
