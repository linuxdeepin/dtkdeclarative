/*
 * Copyright (C) 2020 Uniontech Technology Co., Ltd.
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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef DMASKEFFECTNODE_P_H
#define DMASKEFFECTNODE_P_H

#include "private/qsgtexturematerial_p.h"

#include <dtkdeclarative_global.h>

#include <QSGImageNode>
#include <QPointer>

DQUICK_BEGIN_NAMESPACE

class OpaqueTextureMaterial : public QSGOpaqueTextureMaterial
{
public:
    QSGMaterialType *type() const override;
    QSGMaterialShader *createShader() const override;
    int compare(const QSGMaterial *other) const override;

    void setMaskTexture(QSGTexture *texture);
    QSGTexture *maskTexture() const { return m_maskTexture; }

    void setMaskScale(QVector2D maskScale);
    QVector2D maskScale() const { return m_maskScale; }

    void setMaskOffset(QVector2D maskOffset);
    QVector2D maskOffset() const { return m_maskOffset; }

    void setSourceScale(QVector2D sourceScale);
    QVector2D sourceScale() const { return m_sourceScale; }

private:
    QPointer<QSGTexture> m_maskTexture;
    QVector2D m_maskScale;
    QVector2D m_maskOffset;
    QVector2D m_sourceScale;
};

class TextureMaterial : public OpaqueTextureMaterial
{
public:
    QSGMaterialType *type() const override;
    QSGMaterialShader *createShader() const override;
    int compare(const QSGMaterial *other) const override;
};

class MaskEffectNode : public QSGImageNode
{
public:
    MaskEffectNode();
    ~MaskEffectNode() override;

    void setRect(const QRectF &rect) override;
    QRectF rect() const override;

    void setSourceRect(const QRectF &r) override;
    QRectF sourceRect() const override;

    void setTexture(QSGTexture *texture) override;
    QSGTexture *texture() const override;

    void setFiltering(QSGTexture::Filtering filtering) override;
    QSGTexture::Filtering filtering() const override;

    void setMipmapFiltering(QSGTexture::Filtering filtering) override;
    QSGTexture::Filtering mipmapFiltering() const override;

    void setTextureCoordinatesTransform(TextureCoordinatesTransformMode mode) override;
    TextureCoordinatesTransformMode textureCoordinatesTransform() const override;

    void setOwnsTexture(bool owns) override;
    bool ownsTexture() const override;

    void setAnisotropyLevel(QSGTexture::AnisotropyLevel level);

    void setMaskTexture(QSGTexture *texture);
    void setMaskScale(QVector2D maskScale);
    void setMaskOffset(QVector2D maskOffset);
    void setSourceScale(QVector2D sourceScale);

    QSGTexture::AnisotropyLevel anisotropyLevel() const;
private:
    QSGGeometry m_geometry;
    OpaqueTextureMaterial m_opaque_material;
    TextureMaterial m_material;
    QRectF m_rect;
    QRectF m_sourceRect;
    TextureCoordinatesTransformMode m_texCoordMode;
    uint m_isAtlasTexture : 1;
    uint m_ownsTexture : 1;
    QMetaObject::Connection m_textureDestroy;
};

DQUICK_END_NAMESPACE

#endif // DMASKEFFECTNODE_P_H
