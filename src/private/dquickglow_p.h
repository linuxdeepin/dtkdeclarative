/*
 * Copyright (C) 2021 UnionTech Technology Co., Ltd.
 *
 * Author:     xiaoyaobing <xiaoyaobing@uniontech.com>
 *
 * Maintainer: xiaoyaobing <xiaoyaobing@uniontech.com>
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

#ifndef DQUICKGLOW_P_H
#define DQUICKGLOW_P_H

#include <dtkdeclarative_global.h>
#include <DObject>

#include <QQuickItem>

DQUICK_BEGIN_NAMESPACE

class DQuickGlowPrivate;
class Q_DECL_EXPORT DQuickGlow : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(qreal glowRadius READ glowRadius WRITE setGlowRadius NOTIFY glowRadiusChanged)
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)
    Q_PROPERTY(qreal spread READ spread WRITE setSpread NOTIFY spreadChanged)
    Q_PROPERTY(qreal relativeSizeX READ relativeSizeX WRITE setRelativeSizeX NOTIFY relativeSizeXChanged)
    Q_PROPERTY(qreal relativeSizeY READ relativeSizeY WRITE setRelativeSizeY NOTIFY relativeSizeYChangd)
    Q_PROPERTY(bool fill READ fill WRITE setFill NOTIFY fillChanged)

public:
    explicit DQuickGlow(QQuickItem *parent = nullptr);

    qreal glowRadius() const;
    void setGlowRadius(qreal radius);

    QColor color() const;
    void setColor(const QColor &);

    qreal spread() const;
    void setSpread(qreal radius);

    qreal relativeSizeX() const;
    void setRelativeSizeX(qreal x);

    qreal relativeSizeY() const;
    void setRelativeSizeY(qreal y);

    bool fill();
    void setFill(bool);
Q_SIGNALS:
    void glowRadiusChanged();
    void colorChanged();
    void spreadChanged();
    void relativeSizeXChanged();
    void relativeSizeYChangd();
    void fillChanged();

protected:
    QSGNode *updatePaintNode(QSGNode *, UpdatePaintNodeData *) override;

protected:
    DQuickGlow(DQuickGlowPrivate &dd, QQuickItem *parent = nullptr);

private:
    Q_DISABLE_COPY(DQuickGlow)
    Q_DECLARE_PRIVATE(DQuickGlow)
};

DQUICK_END_NAMESPACE

#endif // DQUICKGLOW_P_H
