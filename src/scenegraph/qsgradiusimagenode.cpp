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

#include "qsgradiusimagenode.h"

#include <QtMath>

QSGradiusImageNode::QSGradiusImageNode()
{
    setGeometry(&m_geometry);
    setMaterial(&m_material);
    setOpaqueMaterial(&m_opaqueMaterial);

    m_cornerNode.setGeometry(&m_cornerGeometry);
    m_cornerNode.setMaterial(&m_cornerMaterial);
    m_cornerGeometry.setDrawingMode(GL_TRIANGLES);
}

void QSGradiusImageNode::setRect(const QRectF &r)
{
    m_dirtyGeometry |= m_rect != r;

    m_rect = r;
}

void QSGradiusImageNode::setRadius(qreal radius)
{
    m_dirtyGeometry |= m_radius != radius;
    if (!(m_radius > 0) && radius > 0) {
        appendChildNode(&m_cornerNode);
    } else if ((m_radius > 0 && !(radius > 0)) || (!(radius > 0) && !(m_radius > 0))) {
        removeChildNode(&m_cornerNode);
    }

    m_radius = radius;
    m_cornerMaterial.setRadius(m_radius);

    m_cornerNode.markDirty(DirtyMaterial);
}

void QSGradiusImageNode::setColor(const QColor &color)
{
    m_color = color;
    m_material.setSpecifyColor(color);
    m_opaqueMaterial.setSpecifyColor(color);
    m_cornerMaterial.setSpecifyColor(color);

    markDirty(DirtyMaterial);
    m_cornerNode.markDirty(DirtyMaterial);
}

void QSGradiusImageNode::setRenderType(DQuickImage::RenderType type)
{
    m_type = type;
    m_material.setRenderType(type);
    m_opaqueMaterial.setRenderType(type);
    m_cornerMaterial.setRenderType(type);

    markDirty(DirtyMaterial);
    m_cornerNode.markDirty(DirtyMaterial);
}

void QSGradiusImageNode::update()
{
    if (m_dirtyGeometry) {
        m_cornerMaterial.setFlag(QSGMaterial::Blending, true);
        updateGeometry();
    }
}

void QSGradiusImageNode::updateGeometry()
{
    Q_ASSERT(!m_targetRect.isEmpty());
    const QSGTexture *t = materialTexture();
    QRectF sr;
    if (t) {
        QRectF sourceRect = t->normalizedTextureSubRect();

        QRectF innerSourceRect(sourceRect.x() + m_innerSourceRect.x() * sourceRect.width(),
                               sourceRect.y() + m_innerSourceRect.y() * sourceRect.height(),
                               m_innerSourceRect.width() * sourceRect.width(),
                               m_innerSourceRect.height() * sourceRect.height());

        bool hasMargins = m_targetRect != m_innerTargetRect;

        int floorLeft = qFloor(m_subSourceRect.left());
        int ceilRight = qCeil(m_subSourceRect.right());
        int floorTop = qFloor(m_subSourceRect.top());
        int ceilBottom = qCeil(m_subSourceRect.bottom());
        int hTiles = ceilRight - floorLeft;
        int vTiles = ceilBottom - floorTop;

        bool hasTiles = hTiles > 1 || vTiles > 1;
        bool fullTexture = innerSourceRect == QRectF(0, 0, 1, 1);

        if (!hasMargins && (!hasTiles || (fullTexture && supportsWrap(t->textureSize())))) {
            if (!fullTexture) {
                sr = QRectF(innerSourceRect.x() + (m_subSourceRect.left() - floorLeft) * innerSourceRect.width(),
                            innerSourceRect.y() + (m_subSourceRect.top() - floorTop) * innerSourceRect.height(),
                            m_subSourceRect.width() * innerSourceRect.width(),
                            m_subSourceRect.height() * innerSourceRect.height());
            } else {
                sr = QRectF(m_subSourceRect.left() - floorLeft, m_subSourceRect.top() - floorTop,
                            m_subSourceRect.width(), m_subSourceRect.height());
            }
            if (m_mirror) {
                qreal oldLeft = sr.left();
                sr.setLeft(sr.right());
                sr.setRight(oldLeft);
            }
        } else {
            // TODO:此处纹理与几何坐标系计算较为麻烦
        }

        if (m_radius > 0) {
            qreal radius = std::min({ m_targetRect.width() / 2, m_targetRect.height() / 2, m_radius });

            m_geometry.allocate(8);
            m_cornerGeometry.allocate(12);

            const float outerL = m_targetRect.left();
            const float innerL = m_targetRect.left() + radius;
            const float innerR = m_targetRect.right() - radius;
            const float outerR = m_targetRect.right();

            const float outerT = m_targetRect.top();
            const float innerT = m_targetRect.top() + radius;
            const float innerB = m_targetRect.bottom() - radius;
            const float outerB = m_targetRect.bottom();

            float textureXRadius = radius * sr.width() / m_targetRect.width();
            float textureYRadius = radius * sr.height() / m_targetRect.height();

            const float outerTL = sr.left();
            const float innerTL = sr.left() + textureXRadius;
            const float innerTR = sr.right() - textureXRadius;
            const float outerTR = sr.right();

            const float outerTT = sr.top();
            const float innerTT = sr.top() + textureYRadius;
            const float innerTB = sr.bottom() - textureYRadius;
            const float outerTB = sr.bottom();

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
            QSGGeometry::updateTexturedRectGeometry(&m_geometry, m_targetRect, sr);
        }
        markDirty(DirtyGeometry);
    }

    m_dirtyGeometry = false;
}

void QSGradiusImageNode::setFiltering(QSGTexture::Filtering filtering)
{
    if (m_material.filtering() == filtering)
        return;

    m_material.setFiltering(filtering);
    m_opaqueMaterial.setFiltering(filtering);
    m_cornerMaterial.setFiltering(filtering);
    markDirty(DirtyMaterial);
}

void QSGradiusImageNode::setMipmapFiltering(QSGTexture::Filtering filtering)
{
    if (m_material.mipmapFiltering() == filtering)
        return;

    m_material.setMipmapFiltering(filtering);
    m_opaqueMaterial.setMipmapFiltering(filtering);
    m_cornerMaterial.setMipmapFiltering(filtering);
    markDirty(DirtyMaterial);
}

void QSGradiusImageNode::setVerticalWrapMode(QSGTexture::WrapMode wrapMode)
{
    if (m_material.verticalWrapMode() == wrapMode)
        return;

    m_material.setVerticalWrapMode(wrapMode);
    m_opaqueMaterial.setVerticalWrapMode(wrapMode);
    m_cornerMaterial.setVerticalWrapMode(wrapMode);
    markDirty(DirtyMaterial);
}

void QSGradiusImageNode::setHorizontalWrapMode(QSGTexture::WrapMode wrapMode)
{
    if (m_material.horizontalWrapMode() == wrapMode)
        return;

    m_material.setHorizontalWrapMode(wrapMode);
    m_opaqueMaterial.setHorizontalWrapMode(wrapMode);
    m_cornerMaterial.setHorizontalWrapMode(wrapMode);
    markDirty(DirtyMaterial);
}

/**
 * @brief QSGradiusImageNode::updateMaterialAntialiasing 抗锯齿暂时先不支持
 */
void QSGradiusImageNode::updateMaterialAntialiasing()
{

}

void QSGradiusImageNode::setMaterialTexture(QSGTexture *texture)
{
    m_material.setTexture(texture);
    m_opaqueMaterial.setTexture(texture);
    m_cornerMaterial.setTexture(texture);
}

QSGTexture *QSGradiusImageNode::materialTexture() const
{
    return m_material.texture();
}

bool QSGradiusImageNode::updateMaterialBlending()
{
    const bool alpha = m_material.flags() & QSGMaterial::Blending;
    if (materialTexture() && alpha != materialTexture()->hasAlphaChannel()) {
        m_material.setFlag(QSGMaterial::Blending, !alpha);
        return true;
    }
    return false;
}

inline static bool isPowerOfTwo(int x)
{
    // Assumption: x >= 1
    return x == (x & -x);
}

bool QSGradiusImageNode::supportsWrap(const QSize &size) const
{
    bool wrapSupported = true;


    QOpenGLContext *ctx = QOpenGLContext::currentContext();
#ifndef QT_OPENGL_ES_2
    if (ctx->isOpenGLES())
#endif
    {
        bool npotSupported = ctx->functions()->hasOpenGLFeature(QOpenGLFunctions::NPOTTextureRepeat);
        const bool isNpot = !isPowerOfTwo(size.width()) || !isPowerOfTwo(size.height());
        wrapSupported = npotSupported || !isNpot;
    }

    return wrapSupported;
}

