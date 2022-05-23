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

#ifndef DQUICKCOLOROVERLAY_P_H
#define DQUICKCOLOROVERLAY_P_H

#include <dtkdeclarative_global.h>

#include <QQuickItem>

DQUICK_BEGIN_NAMESPACE

class DQuickColorOverlayPrivate;
class DQuickColorOverlay : public QQuickItem
{
    Q_OBJECT
    Q_DISABLE_COPY(DQuickColorOverlay)
    Q_DECLARE_PRIVATE(DQuickColorOverlay)
    Q_PROPERTY(QQuickItem *source READ source WRITE setSource NOTIFY sourceChanged)
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)
    Q_PROPERTY(bool cached READ cached WRITE setCached NOTIFY cachedChanged)

public:
    explicit DQuickColorOverlay(QQuickItem *parent = nullptr);

    QColor color() const;
    void setColor(const QColor &color);

    QQuickItem *source() const;
    void setSource(QQuickItem *item);

    void setCached(bool cached);
    bool cached() const;

Q_SIGNALS:
    void colorChanged();
    void sourceChanged();
    void cachedChanged();

protected:
    DQuickColorOverlay(DQuickColorOverlayPrivate &dd, QQuickItem *parent = nullptr);
    QSGNode *updatePaintNode(QSGNode *oldNode, UpdatePaintNodeData *) override;
};

DQUICK_END_NAMESPACE

#endif // DQUICKCOLOROVERLAY_P_H
