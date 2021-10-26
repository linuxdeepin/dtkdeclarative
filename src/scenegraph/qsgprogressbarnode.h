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

#ifndef QSGPROGRESSBARNODE_H
#define QSGPROGRESSBARNODE_H

#include "qsgprogressbartexture.h"

#include <QRectF>
#include <QColor>
#include <QSGGeometry>
#include <QSGGeometryNode>

struct CornerVertex
{
    float m_x;
    float m_y;
    float m_tx;
    float m_ty;
    float m_cx;
    float m_cy;

    void set(float x, float y, float tx, float ty, float cx, float cy)
    {
        m_x = x; m_y = y; m_tx = tx; m_ty = ty; m_cx = cx; m_cy = cy;
    }
};

/**
 * @brief The QSGRectangleNode class 纯色矩形带圆角节点
 */
class QSGProgressBarNode : public QSGGeometryNode
{
public:
    QSGProgressBarNode(Qt::Orientation orientation);

    void setRect(const QRectF &);
    void setRadius(qreal radius);
    void setColor(const QColor &color);
    void setValue(qreal value);

     void updateGeometry();
     bool geometryIsDirty() const { return m_geometryChanged; }

private:
     const QSGGeometry::AttributeSet &cornerAttributes()
     {
         static QSGGeometry::Attribute data[] = {
             QSGGeometry::Attribute::create(0, 2, GL_FLOAT, true),
             QSGGeometry::Attribute::create(1, 2, GL_FLOAT),
             QSGGeometry::Attribute::create(2, 2, GL_FLOAT)
         };
         static QSGGeometry::AttributeSet attributes = { 3, sizeof(CornerVertex), data };
         return attributes;
     }

private:
     OpaqueRectangleTextureMaterial m_opaqueMaterial;
     RectangleTextureMaterial m_material { m_opaqueMaterial };
     CornerRectangleTextureMaterial m_cornerMaterial { m_opaqueMaterial };
     QSGGeometry m_geometry { QSGGeometry::defaultAttributes_TexturedPoint2D(), 0 };
     QSGGeometry m_cornerGeometry { cornerAttributes(), 0 };
     QSGGeometryNode m_cornerNode;

     QRectF m_rect;
     qreal m_radius;
     QColor m_color;
     qreal m_value;

     bool m_geometryChanged = true;
};
#endif // QSGPROGRESSBARNODE_H
