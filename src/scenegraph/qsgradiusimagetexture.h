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

#ifndef QSGRADIUSIMAGETEXTURE_H
#define QSGRADIUSIMAGETEXTURE_H

#include "private/dquickimage.h"

#include <QSGTextureMaterial>

class QSGRadiusImageOpaqueTextureMaterial : public QSGOpaqueTextureMaterial
{
public:
    QSGMaterialType *type() const override;
    QSGMaterialShader *createShader() const override;
    int compare(const QSGMaterial *other) const override;

    QColor specifyColor() const { return m_specifyColor; }
    DQuickImage::RenderType renderType() const { return m_rendertype; }

    void setSpecifyColor(const QColor &color) { m_specifyColor = color; }
    void setRenderType(DQuickImage::RenderType type) {
        m_rendertype = type;

        if (type == DQuickImage::AutoRender)
            m_blendVec = QPointF(1, 0);
        else
            m_blendVec = QPointF(0, 1);
    }

    QPointF blendVec() const { return m_blendVec; }
protected:
    QColor m_specifyColor;
    DQuickImage::RenderType m_rendertype;
    QPointF m_blendVec;
};

class QSGRadiusImageTextureMaterial : public QSGRadiusImageOpaqueTextureMaterial
{
public:
    QSGRadiusImageTextureMaterial(QSGRadiusImageOpaqueTextureMaterial &material)
        : opaqueMaterial(material)
    {}
    QSGMaterialType *type() const override;
    QSGMaterialShader *createShader() const override;
    int compare(const QSGMaterial *other) const override;

    QSGRadiusImageOpaqueTextureMaterial &opaqueMaterial;
};

class CornerRadiusImageTextureMaterial : public QSGRadiusImageTextureMaterial
{
public:
    CornerRadiusImageTextureMaterial(QSGRadiusImageOpaqueTextureMaterial &material)
        : QSGRadiusImageTextureMaterial(material)
    {}
    qreal radius() const { return m_radius; }
    void setRadius(qreal radius) { m_radius = radius; }

    QSGMaterialType *type() const override;
    QSGMaterialShader *createShader() const override;
    int compare(const QSGMaterial *other) const override;

private:
    qreal m_radius = 0;
};

#endif // QSGRADIUSIMAGETEXTURE_H
