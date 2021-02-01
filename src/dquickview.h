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

#ifndef DQUICKVIEW_H
#define DQUICKVIEW_H

#include <QQuickView>
#include <DObject>

#include <dtkdeclarative_global.h>
#include <DQuickWindow>

DQUICK_BEGIN_NAMESPACE

class DQuickViewPrivate;
class DQuickWindowAttached;

class DQuickView : public QQuickView, public DTK_CORE_NAMESPACE::DObject
{
    Q_OBJECT
public:
    explicit DQuickView(QWindow *parent = nullptr);
    ~DQuickView() override;

    DQuickWindowAttached *attached() const;
    static DQuickWindowAttached *qmlAttachedProperties(QObject *object);

private:
    D_DECLARE_PRIVATE(DQuickView)
};

DQUICK_END_NAMESPACE

QML_DECLARE_TYPEINFO(DTK_QUICK_NAMESPACE::DQuickView, QML_HAS_ATTACHED_PROPERTIES)

#endif // DQUICKVIEW_H
