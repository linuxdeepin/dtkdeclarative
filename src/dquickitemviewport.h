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
#ifndef DQUICKITEMVIEWPORT_H
#define DQUICKITEMVIEWPORT_H

#include <QQuickItem>

#include <dtkdeclarative_global.h>
#include <DObject>

DQUICK_BEGIN_NAMESPACE

class DQuickItemViewportPrivate;
class DQuickItemViewport : public QQuickItem, public DCORE_NAMESPACE::DObject
{
    Q_OBJECT
    Q_PROPERTY(QQuickItem* sourceItem READ sourceItem WRITE setSourceItem NOTIFY sourceItemChanged)
    Q_PROPERTY(QPointF sourceOffset READ sourceOffset WRITE setSourceOffset NOTIFY sourceOffsetChanged)
    Q_PROPERTY(float radius READ radius WRITE setRadius NOTIFY radiusChanged)
    D_DECLARE_PRIVATE(DQuickItemViewport)

public:
    explicit DQuickItemViewport(QQuickItem *parent = nullptr);

    QQuickItem* sourceItem() const;
    QPointF sourceOffset() const;
    float radius() const;

public Q_SLOTS:
    void setSourceItem(QQuickItem* sourceItem);
    void setSourceOffset(QPointF sourceOffset);
    void setRadius(float radius);

Q_SIGNALS:
    void sourceItemChanged(QQuickItem* sourceItem);
    void sourceOffsetChanged(QPointF sourceOffset);
    void radiusChanged(float radius);

protected:
    void itemChange(ItemChange, const ItemChangeData &) override;
    QSGNode *updatePaintNode(QSGNode *, UpdatePaintNodeData *) override;
};

DQUICK_END_NAMESPACE

#endif // DQUICKITEMVIEWPORT_H
