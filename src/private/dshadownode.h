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

#ifndef DSHADOWNODE_H
#define DSHADOWNODE_H

#include <dtkdeclarative_global.h>

#include <private/qsgadaptationlayer_p.h>

#include <QSGVertexColorMaterial>
#include <QSGGeometry>

DQUICK_BEGIN_NAMESPACE

class ShadowMaterial : public QSGVertexColorMaterial
{
public:
    ShadowMaterial();

    QColor color() const { return  m_color; }
    void setColor(const QColor &color) { m_color = color; }

    qreal relativeSizeX() const { return m_relativeSizeX; }
    void setRelativeSizeX(qreal x) { m_relativeSizeX = x; }

    qreal relativeSizeY() const { return m_relativeSizeY; }
    void setRelativeSizeY(qreal y) { m_relativeSizeY = y; }

    qreal spread() const { return m_spread; }
    void setSpread(qreal spread) { m_spread = spread; }

    QSGMaterialType *type() const override;
    QSGMaterialShader *createShader() const override;

    int compare(const QSGMaterial *) const override;

private:
    QColor m_color;
    qreal  m_relativeSizeX;
    qreal  m_relativeSizeY;
    qreal  m_spread;
};

struct ShadowVertex
{
    float m_x;
    float m_y;
    float m_tx;
    float m_ty;

    void set(float x, float y, float tx, float ty)
    {
        m_x = x; m_y = y; m_tx = tx; m_ty = ty;
    }
};

class ShadowNode : public QSGVisitableNode
{
public:
    ShadowNode();

    void setRect(const QRectF &);
    void setColor(const QColor &color);
    void setShadowRadius(qreal shadowRadius);
    void setRelativeSizeX(qreal x);
    void setRelativeSizeY(qreal y);
    void setSpread(qreal spread);
    void setFill(bool);
    void update();
    void updateGeometry();
    bool geometryIsDirty() const { return m_geometryChanged; }
    void accept(QSGNodeVisitorEx *visitor) override { if (visitor->visit(this)) visitor->visitChildren(this); visitor->endVisit(this); }
private:
    static const QSGGeometry::AttributeSet &shadowAttributes()
    {
        static QSGGeometry::Attribute data[] = {
            QSGGeometry::Attribute::create(0, 2, GL_FLOAT, true),
            QSGGeometry::Attribute::create(1, 2, GL_FLOAT)
        };
        static QSGGeometry::AttributeSet attributes = { 2, sizeof(ShadowVertex), data };
        return attributes;
    }

private:
    ShadowMaterial m_material;
    QSGGeometry m_geometry { shadowAttributes(), 0 };

    QRectF m_rect;
    bool m_geometryChanged = true;
    qreal m_shadowRadius;
    bool m_fill;
};

DQUICK_END_NAMESPACE

#endif // DSHADOWNODE_H
