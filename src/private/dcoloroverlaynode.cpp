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

#include "dcoloroverlaynode.h"

#include <QPainter>
#include <QQuickItem>

DQUICK_BEGIN_NAMESPACE

DSoftwareColorOverlayNode::DSoftwareColorOverlayNode(QSGTextureProvider *sourceProvider)
    : DSoftwareEffectRenderNode(sourceProvider)
{
}

void DSoftwareColorOverlayNode::setColor(QColor color)
{
    if (color == m_color)
        return;

    m_color = color;
    markChanged();
}

void DSoftwareColorOverlayNode::sync(QQuickItem *item)
{
    m_item = item;
    m_width = item->width();
    m_height = item->height();
    DSoftwareEffectRenderNode::sync(item);
}

QImage DSoftwareColorOverlayNode::process(const QImage &sourceImage, QPainter::RenderHints hints)
{
    QImage device(sourceImage);
    if (Q_UNLIKELY(sourceImage.isNull()))
        return device;

    if (!m_color.isValid() || m_color == Qt::transparent)
        return device;

    QPainter render(&device);
    render.setRenderHints(hints);
    render.setCompositionMode(QPainter::CompositionMode_SourceIn);
    render.fillRect(sourceImage.rect(), m_color);
    return device;
}

DQUICK_END_NAMESPACE
