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

#ifndef DSOFTWARECOLOROVERLAYNODE_H
#define DSOFTWARECOLOROVERLAYNODE_H

#include "dsoftwareeffectrendernode.h"

#include <dtkdeclarative_global.h>

DQUICK_BEGIN_NAMESPACE

class DSoftwareColorOverlayNode : public DSoftwareEffectRenderNode
{
public:
    explicit DSoftwareColorOverlayNode(QSGTextureProvider *sourceProvider);
    void setColor(QColor color);

    void sync(QQuickItem *item) override;
    QRectF rect() const override { return QRect(0, 0, static_cast<int>(m_width), static_cast<int>(m_height)); }
    QImage process(const QImage &sourceImage, QPainter::RenderHints hints) override;

private:
    QColor m_color = Qt::transparent;
    qreal m_width = 0;
    qreal m_height = 0;
    QQuickItem *m_item = nullptr;
};

DQUICK_END_NAMESPACE

#endif  // DSOFTWARECOLOROVERLAYNODE_H
