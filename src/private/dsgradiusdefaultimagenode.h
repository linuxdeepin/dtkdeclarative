// SPDX-FileCopyrightText: 2024 - 2025 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef DSGRADIUSDEFAULTIMAGENODE_H
#define DSGRADIUSDEFAULTIMAGENODE_H

#include <dtkdeclarative_global.h>

#include <private/qsgadaptationlayer_p.h>
#include <private/qsgdefaultinternalimagenode_p.h>

#include <QSGNode>
#include <QSGImageNode>
#include <QSGTexture>

DQUICK_BEGIN_NAMESPACE

class DSGRadiusSmoothTextureMaterial : public QSGOpaqueTextureMaterial
{
public:
    DSGRadiusSmoothTextureMaterial();

protected:
    QSGMaterialType *type() const override;
    QSGMaterialShader *createShader(QSGRendererInterface::RenderMode renderMode) const override;
};

class Q_QUICK_PRIVATE_EXPORT DSGRadiusDefaultImageNode : public QSGInternalImageNode
{
public:
    DSGRadiusDefaultImageNode();

    void setTargetRect(const QRectF &rect) override;
    void setInnerTargetRect(const QRectF &rect) override;
    void setInnerSourceRect(const QRectF &rect) override;
    void setSubSourceRect(const QRectF &rect) override;
    void setTexture(QSGTexture *texture) override;
    void setAntialiasing(bool antialiasing) override;
    void setMirror(bool mirrorHorizontally, bool mirrorVertically) override;
    void update() override;
    void preprocess() override;
    void setMipmapFiltering(QSGTexture::Filtering filtering) override;
    void setFiltering(QSGTexture::Filtering filtering) override;
    void setHorizontalWrapMode(QSGTexture::WrapMode wrapMode) override;
    void setVerticalWrapMode(QSGTexture::WrapMode wrapMode) override;

    void setRadius(qreal radius);
    void setTopLeftRadius(qreal radius);
    void setTopRightRadius(qreal radius);
    void setBottomLeftRadius(qreal radius);
    void setBottomRightRadius(qreal radius);

    static QSGGeometry *updateGeometry(const QRectF &targetRect,
                                       const QRectF &innerTargetRect,
                                       const QRectF &sourceRect,
                                       const QRectF &innerSourceRect,
                                       const QRectF &subSourceRect,
                                       QSGGeometry *geometry,
                                       bool mirrorHorizontally = false,
                                       bool mirrorVertically = false,
                                       bool antialiasing = false);

protected:
    void updateMaterialAntialiasing();
    void setMaterialTexture(QSGTexture *texture);
    QSGTexture *materialTexture() const;
    bool updateMaterialBlending();
    bool supportsWrap(const QSize &size) const;

    void updateGeometry();
    void updateTexturedRadiusGeometry(const QRectF &rect, const QRectF &textureRect);

private:
    QSGGeometry m_geometry;

    QSGDynamicTexture *m_dynamicTexture;
    QSize m_dynamicTextureSize;
    QRectF m_dynamicTextureSubRect;

    QSGOpaqueTextureMaterial m_material;
    QSGTextureMaterial m_materialO;
    DSGRadiusSmoothTextureMaterial m_radiusMaterial;
    QSGSmoothTextureMaterial m_smoothMaterial;

    float m_radius = 0.0f;
    float m_topLeftRadius = -1.0f;
    float m_topRightRadius = -1.0f;
    float m_bottomLeftRadius = -1.0f;
    float m_bottomRightRadius = -1.0f;
    float m_antialiasingWidth = 2;

    QRectF m_targetRect;
    QRectF m_innerTargetRect;
    QRectF m_innerSourceRect;
    QRectF m_subSourceRect;

    uint m_antialiasing : 1;
    uint m_mirrorHorizontally : 1;
    uint m_mirrorVertically : 1;
    uint m_dirtyGeometry : 1;
    uint m_dirtyRadius : 1;
};

DQUICK_END_NAMESPACE

#endif // DSGDEFAULTIMAGENODE_H
