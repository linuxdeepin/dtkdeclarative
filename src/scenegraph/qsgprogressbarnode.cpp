/*
 * Copyright (C) 2020 ~ 2021 Uniontech Software Technology Co.,Ltd.
 *
 * Author:     xiaoyaobing <xiaoyaobing@uniontech.com>
 *
 * Maintainer: xiaoyaobing <xiaoyaobing@uniontech.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "qsgprogressbarnode.h"

QSGProgressBarNode::QSGProgressBarNode(Qt::Orientation orientation)
    : m_radius(-1)
    , m_color(Qt::black)
    , m_value(-1)
{
    setGeometry(&m_geometry);
    m_material.setorientation(orientation);
    m_opaqueMaterial.setorientation(orientation);
    m_cornerMaterial.setorientation(orientation);
    setMaterial(&m_material);
    setOpaqueMaterial(&m_opaqueMaterial);

    m_cornerNode.setGeometry(&m_cornerGeometry);
    m_cornerNode.setMaterial(&m_cornerMaterial);

    m_material.setFlag(QSGMaterial::Blending, true);
    m_opaqueMaterial.setFlag(QSGMaterial::Blending, true);
    m_cornerMaterial.setFlag(QSGMaterial::Blending, true);

    m_cornerGeometry.setDrawingMode(GL_TRIANGLES);
}

void QSGProgressBarNode::setRect(const QRectF &r)
{
    m_geometryChanged |= m_rect != r;

    m_rect = r;
}

void QSGProgressBarNode::setRadius(qreal radius)
{
    if (qFuzzyCompare(m_radius, radius))
        return;

    m_geometryChanged |= m_radius != radius;
    if (!(m_radius > 0) && radius > 0) {
        // 直角矩形到圆角
        appendChildNode(&m_cornerNode);
    } else if ((m_radius > 0 && !(radius > 0)) || (!(radius > 0) && !(m_radius > 0))) {
        // 圆角矩形到直角
        removeChildNode(&m_cornerNode);
    }

    m_radius = radius;
    m_cornerMaterial.setRadius(m_radius);

    m_cornerNode.markDirty(DirtyMaterial);
}

void QSGProgressBarNode::setColor(const QColor &color)
{
    if (m_color == color)
        return;

    m_color = color;
    m_material.setColor(color);
    m_opaqueMaterial.setColor(color);
    m_cornerMaterial.setColor(color);

    markDirty(DirtyMaterial);
    m_cornerNode.markDirty(DirtyMaterial);
}

void QSGProgressBarNode::setValue(qreal value)
{
    if (qFuzzyCompare(m_value, value))
        return;

    m_value = value;

    m_material.setValue(value);
    m_opaqueMaterial.setValue(value);
    m_cornerMaterial.setValue(value);

    markDirty(DirtyMaterial);
    m_cornerNode.markDirty(DirtyMaterial);
}

void QSGProgressBarNode::updateGeometry()
{
    if (m_geometryChanged) {
        m_geometryChanged = false;

        const QRectF textureRect = QRectF(0, 0, 1, 1);

        if (m_radius > 0) {
            qreal radius = std::min({ m_rect.width() / 2, m_rect.height() / 2, m_radius });

            m_geometry.allocate(8);
            m_cornerGeometry.allocate(12);

            const float outerL = m_rect.left();
            const float innerL = m_rect.left() + radius;
            const float innerR = m_rect.right() - radius;
            const float outerR = m_rect.right();

            const float outerT = m_rect.top();
            const float innerT = m_rect.top() + radius;
            const float innerB = m_rect.bottom() - radius;
            const float outerB = m_rect.bottom();

            float textureXRadius = radius * textureRect.width() / m_rect.width();
            float textureYRadius = radius * textureRect.height() / m_rect.height();

            const float outerTL = textureRect.left();
            const float innerTL = textureRect.left() + textureXRadius;
            const float innerTR = textureRect.right() - textureXRadius;
            const float outerTR = textureRect.right();

            const float outerTT = textureRect.top();
            const float innerTT = textureRect.top() + textureYRadius;
            const float innerTB = textureRect.bottom() - textureYRadius;
            const float outerTB = textureRect.bottom();

            QSGGeometry::TexturedPoint2D *vertices = m_geometry.vertexDataAsTexturedPoint2D();

            vertices[0].set(outerL, innerB, outerTL, innerTB); // Outer left, inner bottom
            vertices[1].set(outerL, innerT, outerTL, innerTT); // Outer left, inner top
            vertices[2].set(innerL, outerB, innerTL, outerTB); // Inner left, outer bottom
            vertices[3].set(innerL, outerT, innerTL, outerTT); // Inner left, outer top
            vertices[4].set(innerR, outerB, innerTR, outerTB); // Inner right, outer botton
            vertices[5].set(innerR, outerT, innerTR, outerTT); // Inner right, outer top
            vertices[6].set(outerR, innerB, outerTR, innerTB); // Outer right, inner bottom
            vertices[7].set(outerR, innerT, outerTR, innerTT); // Outer right, inner top

            // Corners
            CornerVertex *corners = static_cast<CornerVertex *>(m_cornerGeometry.vertexData());

            // Bottom left
            corners[0].set(outerL, outerB, outerTL, outerTB, radius, radius);
            corners[1].set(outerL, innerB, outerTL, innerTB, radius, 0);
            corners[2].set(innerL, outerB, innerTL, outerTB, 0, radius);

            // Top left
            corners[3].set(outerL, outerT, outerTL, outerTT, radius, radius);
            corners[4].set(outerL, innerT, outerTL, innerTT, radius, 0);
            corners[5].set(innerL, outerT, innerTL, outerTT, 0, radius);

            // Bottom right
            corners[6].set(outerR, outerB, outerTR, outerTB, radius, radius);
            corners[7].set(outerR, innerB, outerTR, innerTB, radius, 0);
            corners[8].set(innerR, outerB, innerTR, outerTB, 0, radius);

            // Top right
            corners[9].set(outerR, outerT, outerTR, outerTT, radius, radius);
            corners[10].set(outerR, innerT, outerTR, innerTT, radius, 0);
            corners[11].set(innerR, outerT, innerTR, outerTT, 0, radius);

            m_cornerNode.markDirty(DirtyGeometry);
        } else {
            m_geometry.allocate(4);
            QSGGeometry::updateTexturedRectGeometry(&m_geometry, m_rect, textureRect);
        }

        markDirty(DirtyGeometry);
    }
}
