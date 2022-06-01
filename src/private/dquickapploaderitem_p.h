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
#ifndef DQUICKAPPLOADERITEM_H
#define DQUICKAPPLOADERITEM_H

#include <dtkdeclarative_global.h>

#include <DObject>
#include <QQuickItem>

DQUICK_BEGIN_NAMESPACE
class DQuickAppLoaderItemPrivate;
class DQuickAppLoaderItem : public QQuickItem, public DCORE_NAMESPACE::DObject
{
    Q_OBJECT
    D_DECLARE_PRIVATE(DQuickAppLoaderItem)
    Q_PROPERTY(QQuickWindow *window READ window NOTIFY windowChanged)
    Q_PROPERTY(bool loaded READ loaded NOTIFY loadedChanged)
    Q_PROPERTY(qreal progress READ progress NOTIFY progressChanged)
    Q_PROPERTY(bool asynchronous READ asynchronous WRITE setAsynchronous NOTIFY asynchronousChanged)

public:
    explicit DQuickAppLoaderItem(QQuickItem *parentItem = nullptr);

    bool loaded() const;
    void setLoaded(bool loaded);

    qreal progress() const;
    void setProgress(qreal progress);

    QQuickWindow *window() const;
    void setWindow(QQuickWindow *w);

    bool asynchronous() const;
    void setAsynchronous(bool a);
Q_SIGNALS:
    void loadedChanged();
    void progressChanged();
    void windowChanged();
    void asynchronousChanged();
};

DQUICK_END_NAMESPACE
#endif  // DQUICKAPPLOADERITEM_H
