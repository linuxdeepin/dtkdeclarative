/*
 * Copyright (C) 2022 UnionTech Technology Co., Ltd.
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

#ifndef DRECTANGLENODE_P_H
#define DRECTANGLENODE_P_H

#include "dquickrectangle_p.h"

#include <dtkdeclarative_global.h>

#include <private/qsgtexturematerial_p.h>
#include <private/qsgrendernode_p.h>

#include <QSGMaterial>
#include <QSGGeometry>
#include <QSGTextureMaterial>
#include <QSGRectangleNode>
#include <QSGVertexColorMaterial>

DQUICK_BEGIN_NAMESPACE

class CornerColorShader : public QSGOpaqueTextureMaterialShader
{
public:
    const char *vertexShader() const override;
    const char *fragmentShader() const override;
    char const *const *attributeNames() const override;
    void updateState(const RenderState &state, QSGMaterial *newMaterial, QSGMaterial *oldMaterial) override;
    void initialize() override;

private:
    int m_idQtOpacity = -1;
};

class CornerColorMaterial : public QSGOpaqueTextureMaterial
{
public:
    CornerColorMaterial();

    qreal radius() const { return m_radius; }
    void setRadius(qreal radius) { m_radius = radius; }

    QColor color() const { return  m_color; }
    void setColor(const QColor &color) { m_color = color; }

    QSGMaterialType *type() const override;
    QSGMaterialShader *createShader() const override;

    int compare(const QSGMaterial *other) const override;

private:
    qreal m_radius;
    QColor m_color;
};

class DRectangleNode : public QSGRectangleNode
{
public:
    DRectangleNode();

    void setRect(const QRectF &) override;
    QRectF rect() const override { return m_rect; }
    void setRadius(qreal radius);
    void setColor(const QColor &color) override;
    QColor color() const override { return m_cornerMaterial.color(); }
    void setMakTexture(QSGTexture *texture);
    void setCorners(DQuickRectangle::Corners);
    void update();

protected:
    void updateGeometry();

private:
    QSGVertexColorMaterial m_material;
    QSGGeometry m_geometry;

    CornerColorMaterial m_cornerMaterial;
    QSGGeometry m_cornerGeometry;
    QSGGeometryNode m_cornerNode;

    QRectF m_rect;
    bool m_geometryChanged;
    DQuickRectangle::Corners m_coners;
};

class DSoftRectangleNode : public QSGRenderNode
{
public:
    DSoftRectangleNode(QQuickItem *owner);

    StateFlags changedStates() const override;
    void render(const RenderState *state) override;
    RenderingFlags flags() const override;
    QRectF rect() const override;

    void setRadius(qreal radius);
    void setColor(const QColor &color);
    void setCorners(DQuickRectangle::Corners);

private:
    QQuickItem *m_item;
    qreal m_radius;
    QColor m_color;
    DQuickRectangle::Corners m_coners;
    QQuickWindow *m_window = nullptr;
};

DQUICK_END_NAMESPACE

#endif // DRECTANGLENODE_P_H
