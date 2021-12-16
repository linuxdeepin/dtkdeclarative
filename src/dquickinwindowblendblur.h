/*
 * Copyright (C) 2021 UnionTech Technology Co., Ltd.
 *
 * Author:     JiDe Zhang <zhangjide@deepin.org>
 *
 * Maintainer: JiDe Zhang <zhangjide@deepin.org>
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
#ifndef DQUICKINWINDOWBLENDBLUR_H
#define DQUICKINWINDOWBLENDBLUR_H

#include <dtkdeclarative_global.h>
#include <QQuickItem>

DQUICK_BEGIN_NAMESPACE

class DQuickInWindowBlendBlurPrivate;
class DQuickInWindowBlendBlur : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(qreal radius READ radius WRITE setRadius NOTIFY radiusChanged)
    Q_PROPERTY(QColor blendColor READ blendColor WRITE setBlendColor NOTIFY blendColorChanged)

public:
    explicit DQuickInWindowBlendBlur(QQuickItem *parent = nullptr);

    qreal radius() const;
    void setRadius(qreal newRadius);

    const QColor &blendColor() const;
    void setBlendColor(const QColor &newBlendColor);

Q_SIGNALS:
    void radiusChanged();
    void blendColorChanged();

private:
    QSGNode *updatePaintNode(QSGNode *, UpdatePaintNodeData *) override;
    void itemChange(ItemChange, const ItemChangeData &) override;

    qreal m_radius;
    QColor m_blendColor;
};

DQUICK_END_NAMESPACE

#endif // DQUICKINWINDOWBLENDBLUR_H
