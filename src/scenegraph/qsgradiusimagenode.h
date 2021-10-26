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

#ifndef QSGRADIUSIMAGENODE_H
#define QSGRADIUSIMAGENODE_H

#include "qsgprogressbarnode.h"
#include "private/dquickimage.h"
#include "qsgradiusimagetexture.h"

#include <private/qsgbasicinternalimagenode_p.h>
#include <private/qsgdefaultrendercontext_p.h>

class QSGradiusImageNode: public QSGBasicInternalImageNode
{
public:
    QSGradiusImageNode();
    void setRect(const QRectF &);
    void setRadius(qreal radius);
    void setColor(const QColor &color);
    void setRenderType(DQuickImage::RenderType type);
    bool geometryIsDirty() const { return m_dirtyGeometry; }
    void update() override;
    void updateGeometry();

    void setMipmapFiltering(QSGTexture::Filtering filtering) override;
    void setFiltering(QSGTexture::Filtering filtering) override;
    void setHorizontalWrapMode(QSGTexture::WrapMode wrapMode) override;
    void setVerticalWrapMode(QSGTexture::WrapMode wrapMode) override;

    void updateMaterialAntialiasing() override;
    void setMaterialTexture(QSGTexture *texture) override;
    QSGTexture *materialTexture() const override;
    bool updateMaterialBlending() override;
    bool supportsWrap(const QSize &size) const override;

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
    QSGRadiusImageOpaqueTextureMaterial m_opaqueMaterial;
    QSGRadiusImageTextureMaterial m_material { m_opaqueMaterial };
    CornerRadiusImageTextureMaterial m_cornerMaterial { m_opaqueMaterial };

    QSGGeometry m_cornerGeometry { cornerAttributes(), 0 };
    QSGGeometryNode m_cornerNode;
    qreal m_radius = 0;
    QColor m_color;
    DQuickImage::RenderType m_type;
    QRectF m_rect;
};

#endif // QSGRADIUSIMAGENODE_H
