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
#include "dquickapploaderitem_p.h"

#include <DObjectPrivate>

DQUICK_BEGIN_NAMESPACE

class DQuickAppLoaderItemPrivate : public DCORE_NAMESPACE::DObjectPrivate
{
    D_DECLARE_PUBLIC(DQuickAppLoaderItem)
public:
    DQuickAppLoaderItemPrivate(DQuickAppLoaderItem *qq)
        : DObjectPrivate(qq)
        , window(nullptr)
        , loaded(false)
        , asynchronous(false)
        , progress(0)
    {

    }

    QList<QQmlComponent *> childrenComponents;
    QQuickWindow *window;
    bool loaded;
    bool asynchronous;
    qreal progress;
};

DQuickAppLoaderItem::DQuickAppLoaderItem(QQuickItem *parentItem)
    : QQuickItem(parentItem)
    , DObject(*new DQuickAppLoaderItemPrivate(this))
{

}

bool DQuickAppLoaderItem::loaded() const
{
    D_DC(DQuickAppLoaderItem);
    return d->loaded;
}

void DQuickAppLoaderItem::setLoaded(bool loaded)
{
    D_D(DQuickAppLoaderItem);
    if (loaded == d->loaded)
        return;
    d->loaded = loaded;
    Q_EMIT loadedChanged();
}

qreal DQuickAppLoaderItem::progress() const
{
    D_DC(DQuickAppLoaderItem);
    return d->progress;
}

void DQuickAppLoaderItem::setProgress(qreal progress)
{
    D_D(DQuickAppLoaderItem);
    if (qFuzzyCompare(progress, d->progress))
        return;
    d->progress = progress;
    Q_EMIT progressChanged();
}

QQuickWindow *DQuickAppLoaderItem::window() const
{
    D_DC(DQuickAppLoaderItem);
    return d->window;
}

void DQuickAppLoaderItem::setWindow(QQuickWindow *w)
{
    D_D(DQuickAppLoaderItem);
    if (w == d->window)
        return;
    d->window = w;
    Q_EMIT windowChanged();
}


bool DQuickAppLoaderItem::asynchronous() const
{
    D_DC(DQuickAppLoaderItem);
    return d->asynchronous;
}

void DQuickAppLoaderItem::setAsynchronous(bool a)
{
    D_D(DQuickAppLoaderItem);
    if (a == d->asynchronous)
        return;
    d->asynchronous = a;
    Q_EMIT asynchronousChanged();
}

DQUICK_END_NAMESPACE
