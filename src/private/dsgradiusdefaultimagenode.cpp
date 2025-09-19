// SPDX-FileCopyrightText: 2024 - 2025 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include "dsgradiusdefaultimagenode.h"

#include <private/qsgdefaultrendercontext_p.h>

#include <private/qrhi_p.h>
#include <private/qsgmaterialshader_p.h>
#include <private/qsgtexturematerial_p.h>

DQUICK_BEGIN_NAMESPACE

namespace
{
struct ImageVertex {
    float x, y;
    float tx, ty;
    // for vertex geometry
    void set(float nx, float ny, float ntx, float nty) {
        x = ny; y = nx; tx = nty; ty = ntx;
    }
};
struct RadiusImageVertex : public ImageVertex
{
    float alpha;
    void set(float nx, float ny, float ntx, float nty, float nalpha) {
        ImageVertex::set(nx, ny, ntx, nty);
        alpha = nalpha;
    }
};

const QSGGeometry::AttributeSet &radiusImageAttributeSet()
{
    static QSGGeometry::Attribute data[] = {
        QSGGeometry::Attribute::createWithAttributeType(0, 2, QSGGeometry::FloatType, QSGGeometry::PositionAttribute),
        QSGGeometry::Attribute::createWithAttributeType(1, 2, QSGGeometry::FloatType, QSGGeometry::TexCoordAttribute),
        QSGGeometry::Attribute::createWithAttributeType(2, 1, QSGGeometry::FloatType, QSGGeometry::UnknownAttribute),
    };
    static QSGGeometry::AttributeSet attrs = { 3, sizeof(RadiusImageVertex), data };
    return attrs;
}

struct SmoothImageVertex
{
    float x, y, u, v;
    float dx, dy, du, dv;
};

const QSGGeometry::AttributeSet &smoothImageAttributeSet()
{
    static QSGGeometry::Attribute data[] = {
        QSGGeometry::Attribute::createWithAttributeType(0, 2, QSGGeometry::FloatType, QSGGeometry::PositionAttribute),
        QSGGeometry::Attribute::createWithAttributeType(1, 2, QSGGeometry::FloatType, QSGGeometry::TexCoordAttribute),
        QSGGeometry::Attribute::createWithAttributeType(2, 2, QSGGeometry::FloatType, QSGGeometry::TexCoord1Attribute),
        QSGGeometry::Attribute::createWithAttributeType(3, 2, QSGGeometry::FloatType, QSGGeometry::TexCoord2Attribute)
    };
    static QSGGeometry::AttributeSet attrs = { 4, sizeof(SmoothImageVertex), data };
    return attrs;
}

}

class DSmoothTextureMaterialRhiShader : public QSGOpaqueTextureMaterialRhiShader
{
public:
    DSmoothTextureMaterialRhiShader();

    bool updateUniformData(RenderState &state, QSGMaterial *newMaterial, QSGMaterial *oldMaterial) override;
};


DSmoothTextureMaterialRhiShader::DSmoothTextureMaterialRhiShader()
{
    setShaderFileName(VertexStage, QStringLiteral(":/dtk/declarative/shaders_ng/radiussmoothtexture.vert.qsb"));
    setShaderFileName(FragmentStage, QStringLiteral(":/dtk/declarative/shaders_ng/radiussmoothtexture.frag.qsb"));
}

bool DSmoothTextureMaterialRhiShader::updateUniformData(RenderState &state, QSGMaterial *newMaterial, QSGMaterial *oldMaterial)
{
    bool changed = false;
    QByteArray *buf = state.uniformData();

    if (state.isOpacityDirty()) {
        const float opacity = state.opacity();
        memcpy(buf->data() + 64, &opacity, 4);
        changed = true;
    }

    changed |= QSGOpaqueTextureMaterialRhiShader::updateUniformData(state, newMaterial, oldMaterial);

    return changed;
}

DSGRadiusSmoothTextureMaterial::DSGRadiusSmoothTextureMaterial()
{
    setFlag(RequiresFullMatrixExceptTranslate, true);
    setFlag(Blending, true);
}

QSGMaterialType *DSGRadiusSmoothTextureMaterial::type() const
{
    static QSGMaterialType type;
    return &type;
}

QSGMaterialShader *DSGRadiusSmoothTextureMaterial::createShader(QSGRendererInterface::RenderMode renderMode) const
{
    Q_UNUSED(renderMode);
    return new DSmoothTextureMaterialRhiShader();
}

DSGRadiusDefaultImageNode::DSGRadiusDefaultImageNode()
    : m_geometry(QSGGeometry::defaultAttributes_TexturedPoint2D(), 0)
    , m_dynamicTexture(nullptr)
    , m_innerSourceRect(0, 0, 1, 1)
    , m_subSourceRect(0, 0, 1, 1)
    , m_antialiasing(false)
    , m_mirrorHorizontally(false)
    , m_mirrorVertically(false)
    , m_dirtyGeometry(false)
    , m_dirtyRadius(false)
{
    setGeometry(&m_geometry);
    setMaterial(&m_materialO);
    setOpaqueMaterial(&m_material);

#ifdef QSG_RUNTIME_DESCRIPTION
    qsgnode_set_description(this, QLatin1String("dinternalimage"));
#endif
}

void DSGRadiusDefaultImageNode::setTargetRect(const QRectF &rect)
{
    if (rect == m_targetRect)
        return;
    m_targetRect = rect;
    m_dirtyGeometry = true;
}

void DSGRadiusDefaultImageNode::setInnerTargetRect(const QRectF &rect)
{
    if (rect == m_innerTargetRect)
        return;
    m_innerTargetRect = rect;
    m_dirtyGeometry = true;
}

void DSGRadiusDefaultImageNode::setInnerSourceRect(const QRectF &rect)
{
    if (rect == m_innerSourceRect)
        return;
    m_innerSourceRect = rect;
    m_dirtyGeometry = true;
}

void DSGRadiusDefaultImageNode::setSubSourceRect(const QRectF &rect)
{
    if (rect == m_subSourceRect)
        return;
    m_subSourceRect = rect;
    m_dirtyGeometry = true;
}

void DSGRadiusDefaultImageNode::setTexture(QSGTexture *texture)
{
    Q_ASSERT(texture);
    
    setMaterialTexture(texture);
    updateMaterialBlending();
    
    markDirty(DirtyMaterial);
    
    m_dirtyGeometry = true;
}

void DSGRadiusDefaultImageNode::setAntialiasing(bool antialiasing)
{
    
    if (antialiasing == m_antialiasing && !m_dirtyRadius)
        return;
    
    m_antialiasing = antialiasing;
    if (m_antialiasing) {
        if (m_radius > 0
            || m_topLeftRadius > 0
            || m_topRightRadius > 0
            || m_bottomLeftRadius > 0
            || m_bottomRightRadius > 0) {
            setGeometry(new QSGGeometry(radiusImageAttributeSet(), 0));
        } else {
            setGeometry(new QSGGeometry(smoothImageAttributeSet(), 0));
        }
        setFlag(OwnsGeometry, true);
    } else {
        setGeometry(&m_geometry);
        setFlag(OwnsGeometry, false);
    }
    updateMaterialAntialiasing();
    m_dirtyGeometry = true;
    m_dirtyRadius = false;
}

void DSGRadiusDefaultImageNode::setMirror(bool mirrorHorizontally, bool mirrorVertically)
{
    if (mirrorHorizontally == m_mirrorHorizontally && mirrorVertically == m_mirrorVertically)
        return;
    m_mirrorHorizontally = mirrorHorizontally;
    m_mirrorVertically = mirrorVertically;
    m_dirtyGeometry = true;
}


void DSGRadiusDefaultImageNode::update()
{
    if (m_dirtyGeometry)
        updateGeometry();
}

void DSGRadiusDefaultImageNode::preprocess()
{
    bool doDirty = false;
    QSGDynamicTexture *t = qobject_cast<QSGDynamicTexture *>(materialTexture());
    if (t) {
        doDirty = t->updateTexture();
        if (doDirty) {
            // The geometry may need updating. This is expensive however, so do
            // it only when something relevant has changed.
            if (t != m_dynamicTexture
                || t->textureSize() != m_dynamicTextureSize
                || t->normalizedTextureSubRect() != m_dynamicTextureSubRect) {
                updateGeometry();
                m_dynamicTextureSize = t->textureSize();
                m_dynamicTextureSubRect = t->normalizedTextureSubRect();
            }
        }
    }
    m_dynamicTexture = t;
    
    if (updateMaterialBlending())
        doDirty = true;
    
    if (doDirty)
        markDirty(DirtyMaterial);
}

namespace {
struct X { float x, tx; };
struct Y { float y, ty; };
}

static inline void appendQuad(int indexType, void **indexData,
                              int topLeft, int topRight,
                              int bottomLeft, int bottomRight)
{
    if (indexType == QSGGeometry::UnsignedIntType) {
        quint32 *indices = static_cast<quint32 *>(*indexData);
        *indices++ = topLeft;
        *indices++ = bottomLeft;
        *indices++ = bottomRight;
        *indices++ = bottomRight;
        *indices++ = topRight;
        *indices++ = topLeft;
        *indexData = indices;
    } else {
        Q_ASSERT(indexType == QSGGeometry::UnsignedShortType);
        quint16 *indices = static_cast<quint16 *>(*indexData);
        *indices++ = topLeft;
        *indices++ = bottomLeft;
        *indices++ = bottomRight;
        *indices++ = bottomRight;
        *indices++ = topRight;
        *indices++ = topLeft;
        *indexData = indices;
    }
}

QSGGeometry *DSGRadiusDefaultImageNode::updateGeometry(const QRectF &targetRect,
                                                 const QRectF &innerTargetRect,
                                                 const QRectF &sourceRect,
                                                 const QRectF &innerSourceRect,
                                                 const QRectF &subSourceRect,
                                                 QSGGeometry *geometry,
                                                 bool mirrorHorizontally,
                                                 bool mirrorVertically,
                                                 bool antialiasing)
{
    int floorLeft = qFloor(subSourceRect.left());
    int ceilRight = qCeil(subSourceRect.right());
    int floorTop = qFloor(subSourceRect.top());
    int ceilBottom = qCeil(subSourceRect.bottom());
    int hTiles = ceilRight - floorLeft;
    int vTiles = ceilBottom - floorTop;
    
    int hCells = hTiles;
    int vCells = vTiles;
    if (innerTargetRect.width() == 0)
        hCells = 0;
    if (innerTargetRect.left() != targetRect.left())
        ++hCells;
    if (innerTargetRect.right() != targetRect.right())
        ++hCells;
    if (innerTargetRect.height() == 0)
        vCells = 0;
    if (innerTargetRect.top() != targetRect.top())
        ++vCells;
    if (innerTargetRect.bottom() != targetRect.bottom())
        ++vCells;
    
    QVarLengthArray<X, 32> xData(2 * hCells);
    QVarLengthArray<Y, 32> yData(2 * vCells);
    X *xs = xData.data();
    Y *ys = yData.data();
    
    if (innerTargetRect.left() != targetRect.left()) {
        xs[0].x = targetRect.left();
        xs[0].tx = sourceRect.left();
        xs[1].x = innerTargetRect.left();
        xs[1].tx = innerSourceRect.left();
        xs += 2;
    }
    if (innerTargetRect.width() != 0 && hTiles > 0) {
        xs[0].x = innerTargetRect.left();
        xs[0].tx = innerSourceRect.x() + (subSourceRect.left() - floorLeft) * innerSourceRect.width();
        ++xs;
        float b = innerTargetRect.width() / subSourceRect.width();
        float a = innerTargetRect.x() - subSourceRect.x() * b;
        for (int i = floorLeft + 1; i <= ceilRight - 1; ++i) {
            xs[0].x = xs[1].x = a + b * i;
            xs[0].tx = innerSourceRect.right();
            xs[1].tx = innerSourceRect.left();
            xs += 2;
        }
        xs[0].x = innerTargetRect.right();
        xs[0].tx = innerSourceRect.x() + (subSourceRect.right() - ceilRight + 1) * innerSourceRect.width();
        ++xs;
    }
    if (innerTargetRect.right() != targetRect.right()) {
        xs[0].x = innerTargetRect.right();
        xs[0].tx = innerSourceRect.right();
        xs[1].x = targetRect.right();
        xs[1].tx = sourceRect.right();
        xs += 2;
    }
    Q_ASSERT(xs == xData.data() + xData.size());
    if (mirrorHorizontally) {
        float leftPlusRight = targetRect.left() + targetRect.right();
        int count = xData.size();
        xs = xData.data();
        for (int i = 0; i < (count >> 1); ++i)
            qSwap(xs[i], xs[count - 1 - i]);
        for (int i = 0; i < count; ++i)
            xs[i].x = leftPlusRight - xs[i].x;
    }
    
    if (innerTargetRect.top() != targetRect.top()) {
        ys[0].y = targetRect.top();
        ys[0].ty = sourceRect.top();
        ys[1].y = innerTargetRect.top();
        ys[1].ty = innerSourceRect.top();
        ys += 2;
    }
    if (innerTargetRect.height() != 0 && vTiles > 0) {
        ys[0].y = innerTargetRect.top();
        ys[0].ty = innerSourceRect.y() + (subSourceRect.top() - floorTop) * innerSourceRect.height();
        ++ys;
        float b = innerTargetRect.height() / subSourceRect.height();
        float a = innerTargetRect.y() - subSourceRect.y() * b;
        for (int i = floorTop + 1; i <= ceilBottom - 1; ++i) {
            ys[0].y = ys[1].y = a + b * i;
            ys[0].ty = innerSourceRect.bottom();
            ys[1].ty = innerSourceRect.top();
            ys += 2;
        }
        ys[0].y = innerTargetRect.bottom();
        ys[0].ty = innerSourceRect.y() + (subSourceRect.bottom() - ceilBottom + 1) * innerSourceRect.height();
        ++ys;
    }
    if (innerTargetRect.bottom() != targetRect.bottom()) {
        ys[0].y = innerTargetRect.bottom();
        ys[0].ty = innerSourceRect.bottom();
        ys[1].y = targetRect.bottom();
        ys[1].ty = sourceRect.bottom();
        ys += 2;
    }
    Q_ASSERT(ys == yData.data() + yData.size());
    if (mirrorVertically) {
        float topPlusBottom = targetRect.top() + targetRect.bottom();
        int count = yData.size();
        ys = yData.data();
        for (int i = 0; i < (count >> 1); ++i)
            qSwap(ys[i], ys[count - 1 - i]);
        for (int i = 0; i < count; ++i)
            ys[i].y = topPlusBottom - ys[i].y;
    }
    
    QSGGeometry::Type indexType = QSGGeometry::UnsignedShortType;
    // We can handled up to 0xffff indices, but keep the limit lower here to
    // merge better in the batch renderer.
    if (hCells * vCells * 4 > 0x7fff)
        indexType = QSGGeometry::UnsignedIntType;
    
    if (antialiasing) {
        if (!geometry || geometry->indexType() != indexType) {
            geometry = new QSGGeometry(radiusImageAttributeSet(),
                                       hCells * vCells * 4 + (hCells + vCells - 1) * 4,
                                       hCells * vCells * 6 + (hCells + vCells) * 12,
                                       indexType);
        } else {
            geometry->allocate(hCells * vCells * 4 + (hCells + vCells - 1) * 4,
                               hCells * vCells * 6 + (hCells + vCells) * 12);
        }
        QSGGeometry *g = geometry;
        Q_ASSERT(g);
        
        g->setDrawingMode(QSGGeometry::DrawTriangles);
        auto *vertices = reinterpret_cast<SmoothImageVertex *>(g->vertexData());
        memset(vertices, 0, g->vertexCount() * g->sizeOfVertex());
        void *indexData = g->indexData();
        
        // The deltas are how much the fuzziness can reach into the image.
        // Only the border vertices are moved by the vertex shader, so the fuzziness
        // can't reach further into the image than the closest interior vertices.
        float leftDx = xData.at(1).x - xData.at(0).x;
        float rightDx = xData.at(xData.size() - 1).x - xData.at(xData.size() - 2).x;
        float topDy = yData.at(1).y - yData.at(0).y;
        float bottomDy = yData.at(yData.size() - 1).y - yData.at(yData.size() - 2).y;
        
        float leftDu = xData.at(1).tx - xData.at(0).tx;
        float rightDu = xData.at(xData.size() - 1).tx - xData.at(xData.size() - 2).tx;
        float topDv = yData.at(1).ty - yData.at(0).ty;
        float bottomDv = yData.at(yData.size() - 1).ty - yData.at(yData.size() - 2).ty;
        
        if (hCells == 1) {
            leftDx = rightDx *= 0.5f;
            leftDu = rightDu *= 0.5f;
        }
        if (vCells == 1) {
            topDy = bottomDy *= 0.5f;
            topDv = bottomDv *= 0.5f;
        }
        
        // This delta is how much the fuzziness can reach out from the image.
        float delta = float(qAbs(targetRect.width()) < qAbs(targetRect.height())
                                ? targetRect.width() : targetRect.height()) * 0.5f;
        
        int index = 0;
        ys = yData.data();
        for (int j = 0; j < vCells; ++j, ys += 2) {
            xs = xData.data();
            bool isTop = j == 0;
            bool isBottom = j == vCells - 1;
            for (int i = 0; i < hCells; ++i, xs += 2) {
                bool isLeft = i == 0;
                bool isRight = i == hCells - 1;
                
                SmoothImageVertex *v = vertices + index;
                
                int topLeft = index;
                for (int k = (isTop || isLeft ? 2 : 1); k--; ++v, ++index) {
                    v->x = xs[0].x;
                    v->u = xs[0].tx;
                    v->y = ys[0].y;
                    v->v = ys[0].ty;
                }
                
                int topRight = index;
                for (int k = (isTop || isRight ? 2 : 1); k--; ++v, ++index) {
                    v->x = xs[1].x;
                    v->u = xs[1].tx;
                    v->y = ys[0].y;
                    v->v = ys[0].ty;
                }
                
                int bottomLeft = index;
                for (int k = (isBottom || isLeft ? 2 : 1); k--; ++v, ++index) {
                    v->x = xs[0].x;
                    v->u = xs[0].tx;
                    v->y = ys[1].y;
                    v->v = ys[1].ty;
                }
                
                int bottomRight = index;
                for (int k = (isBottom || isRight ? 2 : 1); k--; ++v, ++index) {
                    v->x = xs[1].x;
                    v->u = xs[1].tx;
                    v->y = ys[1].y;
                    v->v = ys[1].ty;
                }
                
                appendQuad(g->indexType(), &indexData, topLeft, topRight, bottomLeft, bottomRight);
                
                if (isTop) {
                    vertices[topLeft].dy = vertices[topRight].dy = topDy;
                    vertices[topLeft].dv = vertices[topRight].dv = topDv;
                    vertices[topLeft + 1].dy = vertices[topRight + 1].dy = -delta;
                    appendQuad(g->indexType(), &indexData, topLeft + 1, topRight + 1, topLeft, topRight);
                }
                
                if (isBottom) {
                    vertices[bottomLeft].dy = vertices[bottomRight].dy = -bottomDy;
                    vertices[bottomLeft].dv = vertices[bottomRight].dv = -bottomDv;
                    vertices[bottomLeft + 1].dy = vertices[bottomRight + 1].dy = delta;
                    appendQuad(g->indexType(), &indexData, bottomLeft, bottomRight, bottomLeft + 1, bottomRight + 1);
                }
                
                if (isLeft) {
                    vertices[topLeft].dx = vertices[bottomLeft].dx = leftDx;
                    vertices[topLeft].du = vertices[bottomLeft].du = leftDu;
                    vertices[topLeft + 1].dx = vertices[bottomLeft + 1].dx = -delta;
                    appendQuad(g->indexType(), &indexData, topLeft + 1, topLeft, bottomLeft + 1, bottomLeft);
                }
                
                if (isRight) {
                    vertices[topRight].dx = vertices[bottomRight].dx = -rightDx;
                    vertices[topRight].du = vertices[bottomRight].du = -rightDu;
                    vertices[topRight + 1].dx = vertices[bottomRight + 1].dx = delta;
                    appendQuad(g->indexType(), &indexData, topRight, topRight + 1, bottomRight, bottomRight + 1);
                }
            }
        }
        
        Q_ASSERT(index == g->vertexCount());
    } else {
        if (!geometry || geometry->indexType() != indexType) {
            geometry = new QSGGeometry(QSGGeometry::defaultAttributes_TexturedPoint2D(),
                                       hCells * vCells * 4, hCells * vCells * 6,
                                       indexType);
        } else {
            geometry->allocate(hCells * vCells * 4, hCells * vCells * 6);
        }
        geometry->setDrawingMode(QSGGeometry::DrawTriangles);
        QSGGeometry::TexturedPoint2D *vertices = geometry->vertexDataAsTexturedPoint2D();
        ys = yData.data();
        for (int j = 0; j < vCells; ++j, ys += 2) {
            xs = xData.data();
            for (int i = 0; i < hCells; ++i, xs += 2) {
                vertices[0].x = vertices[2].x = xs[0].x;
                vertices[0].tx = vertices[2].tx = xs[0].tx;
                vertices[1].x = vertices[3].x = xs[1].x;
                vertices[1].tx = vertices[3].tx = xs[1].tx;
                
                vertices[0].y = vertices[1].y = ys[0].y;
                vertices[0].ty = vertices[1].ty = ys[0].ty;
                vertices[2].y = vertices[3].y = ys[1].y;
                vertices[2].ty = vertices[3].ty = ys[1].ty;
                
                vertices += 4;
            }
        }
        void *indexData = geometry->indexData();
        for (int i = 0; i < 4 * vCells * hCells; i += 4)
            appendQuad(geometry->indexType(), &indexData, i, i + 1, i + 2, i + 3);
    }
    return geometry;
}

void DSGRadiusDefaultImageNode::updateGeometry()
{
    Q_ASSERT(!m_targetRect.isEmpty());
    const QSGTexture *t = materialTexture();
    if (!t)
        return;
    
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
    
    // An image can be rendered as a single quad if:
    // - There are no margins, and either:
    //   - the image isn't repeated
    //   - the source rectangle fills the entire texture so that texture wrapping can be used,
    //     and NPOT is supported
    if (!hasMargins && (!hasTiles || (fullTexture && supportsWrap(t->textureSize())))) {
        QRectF sr;
        if (!fullTexture) {
            sr = QRectF(innerSourceRect.x() + (m_subSourceRect.left() - floorLeft) * innerSourceRect.width(),
                        innerSourceRect.y() + (m_subSourceRect.top() - floorTop) * innerSourceRect.height(),
                        m_subSourceRect.width() * innerSourceRect.width(),
                        m_subSourceRect.height() * innerSourceRect.height());
        } else {
            sr = QRectF(m_subSourceRect.left() - floorLeft, m_subSourceRect.top() - floorTop,
                        m_subSourceRect.width(), m_subSourceRect.height());
        }
        if (m_mirrorHorizontally) {
            qreal oldLeft = sr.left();
            sr.setLeft(sr.right());
            sr.setRight(oldLeft);
        }
        if (m_mirrorVertically) {
            qreal oldTop = sr.top();
            sr.setTop(sr.bottom());
            sr.setBottom(oldTop);
        }

        if (m_radius > 0
            || m_topLeftRadius > 0
            || m_topRightRadius > 0
            || m_bottomLeftRadius > 0
            || m_bottomRightRadius > 0) {
            updateTexturedRadiusGeometry(m_targetRect, sr);
        } else {
            if (m_antialiasing) {
                QSGGeometry *g = geometry();
                Q_ASSERT(g != &m_geometry);
                if (g->indexType() != QSGGeometry::UnsignedShortType) {
                    setGeometry(new QSGGeometry(smoothImageAttributeSet(), 0));
                    g = geometry();
                }
                g->allocate(8, 14);
                g->setDrawingMode(QSGGeometry::DrawTriangleStrip);
                auto *vertices = reinterpret_cast<SmoothImageVertex *>(g->vertexData());
                float delta = float(qAbs(m_targetRect.width()) < qAbs(m_targetRect.height())
                                        ? m_targetRect.width() : m_targetRect.height()) * 0.5f;
                float sx = float(sr.width() / m_targetRect.width());
                float sy = float(sr.height() / m_targetRect.height());
                for (int d = -1; d <= 1; d += 2) {
                    for (int j = 0; j < 2; ++j) {
                        for (int i = 0; i < 2; ++i, ++vertices) {
                            vertices->x = m_targetRect.x() + i * m_targetRect.width();
                            vertices->y = m_targetRect.y() + j * m_targetRect.height();
                            vertices->u = sr.x() + i * sr.width();
                            vertices->v = sr.y() + j * sr.height();
                            vertices->dx = (i == 0 ? delta : -delta) * d;
                            vertices->dy = (j == 0 ? delta : -delta) * d;
                            vertices->du = (d < 0 ? 0 : vertices->dx * sx);
                            vertices->dv = (d < 0 ? 0 : vertices->dy * sy);
                        }
                    }
                }
                Q_ASSERT(vertices - g->vertexCount() == g->vertexData());
                static const quint16 indices[] = {
                    0, 4, 1, 5, 3, 7, 2, 6, 0, 4,
                    4, 6, 5, 7
                };
                Q_ASSERT(g->sizeOfIndex() * g->indexCount() == sizeof(indices));
                memcpy(g->indexDataAsUShort(), indices, sizeof(indices));
            } else {
                m_geometry.allocate(4);
                m_geometry.setDrawingMode(QSGGeometry::DrawTriangleStrip);
                QSGGeometry::updateTexturedRectGeometry(&m_geometry, m_targetRect, sr);
            }
        }
    } else {
        QSGGeometry *g = geometry();
        g->allocate(4);
        g->setDrawingMode(QSGGeometry::DrawTriangleStrip);
        memset(g->vertexData(), 0, g->sizeOfVertex() * 4);
        g = updateGeometry(m_targetRect, m_innerTargetRect,
                           sourceRect, innerSourceRect, m_subSourceRect,
                           g, m_mirrorHorizontally, m_mirrorVertically, false);
        if (g != geometry()) {
            setGeometry(g);
            setFlag(OwnsGeometry, true);
        }
    }
    markDirty(DirtyGeometry);
    m_dirtyGeometry = false;
}

void DSGRadiusDefaultImageNode::updateTexturedRadiusGeometry(const QRectF &rect, const QRectF &textureRect)
{
    float width = float(rect.width());
    float height = float(rect.height());
    
    QSGGeometry *g = geometry();
    g->setDrawingMode(QSGGeometry::DrawTriangleStrip);
    int vertexStride = g->sizeOfVertex();
    
    union {
        ImageVertex *vertices;
        RadiusImageVertex *smoothVertices;
    };
    
    float radiusTL = qMin(qMin(width, height) * 0.4999f, float(m_topLeftRadius < 0 ? m_radius : m_topLeftRadius));
    float radiusTR = qMin(qMin(width, height) * 0.4999f, float(m_topRightRadius < 0 ? m_radius : m_topRightRadius));
    float radiusBL = qMin(qMin(width, height) * 0.4999f, float(m_bottomLeftRadius < 0 ? m_radius : m_bottomLeftRadius));
    float radiusBR = qMin(qMin(width, height) * 0.4999f, float(m_bottomRightRadius < 0 ? m_radius : m_bottomRightRadius));
    
    if (radiusTL <= 0.5)
        radiusTL = 0;
    if (radiusTR <= 0.5)
        radiusTR = 0;
    if (radiusBL <= 0.5)
        radiusBL = 0;
    if (radiusBR <= 0.5)
        radiusBR = 0;
    
    const float innerRadiusTL = qMax(radiusTL, 0.01);
    const float innerRadiusTR = qMax(radiusTR, 0.01);
    const float innerRadiusBL = qMax(radiusBL, 0.01);
    const float innerRadiusBR = qMax(radiusBR, 0.01);
    const float outerRadiusTL = radiusTL;
    const float outerRadiusTR = radiusTR;
    const float outerRadiusBL = radiusBL;
    const float outerRadiusBR = radiusBR;
    
    int segmentsTL = radiusTL == 0 ? 0 : qBound(3, qCeil(radiusTL * (M_PI / 6)), 18);
    int segmentsTR = radiusTR == 0 ? 0 : qBound(3, qCeil(radiusTR * (M_PI / 6)), 18);
    int segmentsBL = radiusBL == 0 ? 0 : qBound(3, qCeil(radiusBL * (M_PI / 6)), 18);
    int segmentsBR = radiusBR == 0 ? 0 : qBound(3, qCeil(radiusBR * (M_PI / 6)), 18);
    
    if (innerRadiusTL == innerRadiusTR) {
        if (segmentsTL <= segmentsTR)
            segmentsTL = 0;
        else
            segmentsTR = 0;
    }
    if (innerRadiusBL == innerRadiusBR){
        if (segmentsBL <= segmentsBR)
            segmentsBL = 0;
        else
            segmentsBR = 0;
    }
    
    const int sumSegments = segmentsTL + segmentsTR + segmentsBL + segmentsBR;
    const int innerVertexCount = (sumSegments + 4) * 2;
    int vertexCount = innerVertexCount;
    if (m_antialiasing)
        vertexCount += innerVertexCount;
    
    const int fillIndexCount = innerVertexCount;
    const int innerAAIndexCount = innerVertexCount * 2 + 2;
    int indexCount = 0;
    int fillHead = 0;
    int innerAAHead = 0;
    int innerAATail = 0;
    bool hasFill = true;
    if (hasFill)
        indexCount += fillIndexCount;
    if (m_antialiasing) {
        innerAATail = innerAAHead = indexCount + (innerAAIndexCount >> 1) + 1;
        indexCount += innerAAIndexCount;
    }
    
    g->allocate(vertexCount, indexCount);
    vertices = reinterpret_cast<ImageVertex *>(g->vertexData());
    memset(vertices, 0, vertexCount * vertexStride);
    quint16 *indices = g->indexDataAsUShort();
    quint16 index = 0;
    
    float innerXPrev = 0.;
    float innerYLeftPrev = 0.;
    float innerYRightPrev = 0.;
    
    const float angleTL = 0.5f * float(M_PI) / segmentsTL;
    const float cosStepTL = qFastCos(angleTL);
    const float sinStepTL = qFastSin(angleTL);
    const float angleTR = 0.5f * float(M_PI) / segmentsTR;
    const float cosStepTR = qFastCos(angleTR);
    const float sinStepTR = qFastSin(angleTR);
    const float angleBL = 0.5f * float(M_PI) / segmentsBL;
    const float cosStepBL = qFastCos(angleBL);
    const float sinStepBL = qFastSin(angleBL);
    const float angleBR = 0.5f * float(M_PI) / segmentsBR;
    const float cosStepBR = qFastCos(angleBR);
    const float sinStepBR = qFastSin(angleBR);
    
    const float outerXCenter[][2] = {{
                                         float(rect.top()  + radiusTL),
                                         float(rect.top() + radiusTR)
                                     }, {
                                         float(rect.bottom() - radiusBL),
                                         float(rect.bottom() - radiusBR)
                                     }};
    
    const float outerYCenter[][2] = {{
                                         float(rect.left() + outerRadiusTL),
                                         float(rect.right() - outerRadiusTR)
                                     }, {
                                         float(rect.left() + outerRadiusBL),
                                         float(rect.right() - outerRadiusBR)
                                     }};
    
    const float innerXCenter[][2] = { {
                                         float(rect.top()  + innerRadiusTL),
                                         float(rect.top() + innerRadiusTR)
                                     }, {
                                         float(rect.bottom() - innerRadiusBL),
                                         float(rect.bottom() - innerRadiusBR)
                                     }};
    
    const float innerYCenter[][2] = { {
                                         float(rect.left() + innerRadiusTL),
                                         float(rect.right() - innerRadiusTR)
                                     },{
                                         float(rect.left() + innerRadiusBL),
                                         float(rect.right() - innerRadiusBR)
                                     }};
    
    const float innerRadius[][2] = {{
                                        innerRadiusTL, innerRadiusTR
                                    }, {
                                        innerRadiusBL, innerRadiusBR
                                    }};
    
    const float outerRadius[][2] = {{
                                        outerRadiusTL, outerRadiusTR
                                    }, {
                                        outerRadiusBL, outerRadiusBR
                                    }};
    
    const int segments[][2] = {{
                                   segmentsTL, segmentsTR
                               }, {
                                   segmentsBL, segmentsBR
                               }};
    
    const float cosStep[][2] = {{
                                    cosStepTL, cosStepTR
                                }, {
                                    cosStepBL, cosStepBR
                                }};
    
    const float sinStep[][2] = {{
                                    sinStepTL, sinStepTR
                                }, {
                                    sinStepBL, sinStepBR
                                }};
    
    float cosSegmentAngleLeft;
    float sinSegmentAngleLeft;
    float cosSegmentAngleRight;
    float sinSegmentAngleRight;
    bool advanceLeft = true;
    
    float xLeft, yLeft, xRight, yRight;
    float outerXLeft, outerYLeft, outerXRight, outerYRight;
    float sinAngleLeft, cosAngleLeft, sinAngleRight, cosAngleRight;
    
    float tmpXRight, tmpYRight, tmpXLeft, tmpYLeft;
    float antialiasingWidth = 0.49f * m_antialiasingWidth;
    
    qreal tmpLeft, tmpRight;
    for (int part = 0; part < 2; ++part) {
        cosSegmentAngleLeft = 1. - part;
        sinSegmentAngleLeft = part;
        cosSegmentAngleRight = 1. - part;
        sinSegmentAngleRight = part;
        advanceLeft = true;
        
        for (int iLeft = 0, iRight = 0; iLeft <= segments[part][0] || iRight <= segments[part][1]; ) {
            xLeft = innerXCenter[part][0] - innerRadius[part][0] * cosSegmentAngleLeft;
            xRight = innerXCenter[part][1] - innerRadius[part][1] * cosSegmentAngleRight;
            
            yLeft = innerYCenter[part][0] - innerRadius[part][0] * sinSegmentAngleLeft;
            yRight = innerYCenter[part][1] + innerRadius[part][1] * sinSegmentAngleRight;
            
            if ((iLeft <= segments[part][0] && xLeft <= xRight) || iRight > segments[part][1]) {
                advanceLeft = true;
            } else {
                advanceLeft = false;
            }
            
            if (innerRadius[part][0] == innerRadius[part][1]) {
                if (advanceLeft) {
                    if (outerRadius[part][0] == 0) {
                        sinAngleLeft = 1.;
                        cosAngleLeft = part ? -1. : 1.;
                    } else {
                        sinAngleLeft = sinSegmentAngleLeft;
                        cosAngleLeft = cosSegmentAngleLeft;
                    }
                    if (outerRadius[part][1] == 0) {
                        sinAngleRight = 1.;
                        cosAngleRight = part ? -1. : 1.;
                    } else {
                        sinAngleRight = sinSegmentAngleLeft;
                        cosAngleRight = cosSegmentAngleLeft;
                    }
                    xRight = xLeft;
                    yRight = innerYCenter[part][1] + innerRadius[part][1] * sinAngleRight;
                } else {
                    if (outerRadius[part][0] == 0) {
                        sinAngleLeft = 1.;
                        cosAngleLeft = part ? -1. : 1.;
                    } else {
                        sinAngleLeft = sinSegmentAngleRight;
                        cosAngleLeft = cosSegmentAngleRight;
                    }
                    if (outerRadius[part][1] == 0) {
                        sinAngleRight = 1.;
                        cosAngleRight = part ? -1. : 1.;
                    } else {
                        sinAngleRight = sinSegmentAngleRight;
                        cosAngleRight = cosSegmentAngleRight;
                    }
                    xLeft = xRight;
                    yLeft = innerYCenter[part][0] - innerRadius[part][0] * sinAngleLeft;
                }
            } else if (advanceLeft) {
                if (outerRadius[part][0] == 0) {
                    sinAngleLeft = 1.;
                    cosAngleLeft = part ? -1. : 1.;
                } else {
                    sinAngleLeft = sinSegmentAngleLeft;
                    cosAngleLeft = cosSegmentAngleLeft;
                }
                if (outerRadius[part][1] == 0) {
                    sinAngleRight = 1.;
                    cosAngleRight = part ? -1. : 1.;
                    xRight = xLeft;
                    yRight = innerYCenter[part][1] + innerRadius[part][1] * sinAngleRight;
                } else if (xLeft >= innerXCenter[0][1] && xLeft <= innerXCenter[1][1]) {
                    sinAngleRight = 1.;
                    cosAngleRight = 0.;
                    xRight = xLeft;
                    yRight = innerYCenter[part][1] + innerRadius[part][1] * sinAngleRight;
                } else {
                    if (xRight != innerXPrev) {
                        float t = (xLeft - innerXPrev) / (xRight - innerXPrev);
                        yRight = innerYRightPrev * (1. - t) + yRight * t;
                        xRight = xLeft;
                    }
                    sinAngleRight = (yRight - innerYCenter[part][1]) / innerRadius[part][1];
                    cosAngleRight = -(xRight - innerXCenter[part][1]) / innerRadius[part][1];
                }
            } else {
                if (outerRadius[part][1] == 0) {
                    sinAngleRight = 1.;
                    cosAngleRight = part ? -1. : 1.;
                } else {
                    sinAngleRight = sinSegmentAngleRight;
                    cosAngleRight = cosSegmentAngleRight;
                }
                if (outerRadius[part][0] == 0) {
                    sinAngleLeft = 1.;
                    cosAngleLeft = part ? -1. : 1.;
                    xLeft = xRight;
                    yLeft = innerYCenter[part][0] - innerRadius[part][0] * sinAngleLeft;
                } else if (xRight >= innerXCenter[0][0] && xRight <= innerXCenter[1][0]) {
                    sinAngleLeft = 1.;
                    cosAngleLeft = 0.;
                    xLeft = xRight;
                    yLeft = innerYCenter[part][0] - innerRadius[part][0] * sinAngleLeft;
                } else {
                    if (xLeft != innerXPrev) {
                        float t = (xRight - innerXPrev) / (xLeft - innerXPrev);
                        yLeft = innerYLeftPrev * (1. - t) + yLeft * t;
                        xLeft = xRight;
                    }
                    sinAngleLeft = -(yLeft - innerYCenter[part][0]) / innerRadius[part][0];
                    cosAngleLeft = -(xLeft - innerXCenter[part][0]) / innerRadius[part][0];
                }
            }
            
            outerXLeft = outerXCenter[part][0]  - outerRadius[part][0] * cosAngleLeft;
            outerYLeft = outerYCenter[part][0] - outerRadius[part][0] * sinAngleLeft;
            outerXRight = outerXCenter[part][1] - outerRadius[part][1] * cosAngleRight;
            outerYRight = outerYCenter[part][1] + outerRadius[part][1] * sinAngleRight;
            
            if (hasFill) {
                indices[fillHead++] = index;
                indices[fillHead++] = index + 1;
            }
            
            if (m_antialiasing) {
                indices[--innerAAHead] = index + 2;
                indices[--innerAAHead] = index;
                indices[innerAATail++] = index + 1;
                indices[innerAATail++] = index + 3;
                
                smoothVertices[index++].set(outerXRight, outerYRight, outerXRight / width, outerYRight / height, 1.0f);
                smoothVertices[index++].set(outerXLeft, outerYLeft, outerXLeft / width, outerYLeft / height, 1.0f);
                tmpXRight = xRight - antialiasingWidth * cosAngleRight;
                tmpYRight = yRight + antialiasingWidth * sinAngleRight;
                tmpXLeft = xLeft - antialiasingWidth * cosAngleLeft;
                tmpYLeft = yLeft- antialiasingWidth * sinAngleLeft;
                smoothVertices[index++].set(tmpXRight, tmpYRight, tmpXRight / width, tmpYRight / height, 0.0f);
                smoothVertices[index++].set(tmpXLeft, tmpYLeft, tmpXLeft / width, tmpYLeft / height, 0.0f);
            } else {
                vertices[index++].set(xRight, yRight, xRight / width, yRight / height);
                vertices[index++].set(xLeft, yLeft, xLeft / width, yLeft / height);
            }
            
            innerXPrev = xLeft;
            innerYLeftPrev = yLeft;
            innerYRightPrev = yRight;
            
            if (advanceLeft) {
                iLeft++;
                tmpLeft = cosSegmentAngleLeft;
                cosSegmentAngleLeft = cosSegmentAngleLeft * cosStep[part][0] - sinSegmentAngleLeft * sinStep[part][0];
                sinSegmentAngleLeft = sinSegmentAngleLeft * cosStep[part][0] + tmpLeft * sinStep[part][0];
            } else {
                iRight++;
                tmpRight = cosSegmentAngleRight;
                cosSegmentAngleRight = cosSegmentAngleRight * cosStep[part][1] - sinSegmentAngleRight * sinStep[part][1];
                sinSegmentAngleRight = sinSegmentAngleRight * cosStep[part][1] + tmpRight * sinStep[part][1];
            }
        }
    }
    
    Q_ASSERT(index == vertexCount);
    
    if (m_antialiasing) {
        indices[--innerAAHead] = indices[innerAATail - 1];
        indices[--innerAAHead] = indices[innerAATail - 2];
        Q_ASSERT(innerAATail <= indexCount);
    }
}

void DSGRadiusDefaultImageNode::setFiltering(QSGTexture::Filtering filtering)
{
    if (m_material.filtering() == filtering)
        return;
    
    m_material.setFiltering(filtering);
    m_materialO.setFiltering(filtering);
    m_radiusMaterial.setFiltering(filtering);
    m_smoothMaterial.setFiltering(filtering);
    markDirty(DirtyMaterial);
}

void DSGRadiusDefaultImageNode::setMipmapFiltering(QSGTexture::Filtering filtering)
{
    if (m_material.mipmapFiltering() == filtering)
        return;
    
    m_material.setMipmapFiltering(filtering);
    m_materialO.setMipmapFiltering(filtering);
    m_radiusMaterial.setMipmapFiltering(filtering);
    m_smoothMaterial.setMipmapFiltering(filtering);
    markDirty(DirtyMaterial);
}

void DSGRadiusDefaultImageNode::setVerticalWrapMode(QSGTexture::WrapMode wrapMode)
{
    if (m_material.verticalWrapMode() == wrapMode)
        return;
    
    m_material.setVerticalWrapMode(wrapMode);
    m_materialO.setVerticalWrapMode(wrapMode);
    m_radiusMaterial.setVerticalWrapMode(wrapMode);
    m_smoothMaterial.setVerticalWrapMode(wrapMode);
    markDirty(DirtyMaterial);
}

void DSGRadiusDefaultImageNode::setHorizontalWrapMode(QSGTexture::WrapMode wrapMode)
{
    if (m_material.horizontalWrapMode() == wrapMode)
        return;
    
    m_material.setHorizontalWrapMode(wrapMode);
    m_materialO.setHorizontalWrapMode(wrapMode);
    m_radiusMaterial.setHorizontalWrapMode(wrapMode);
    m_smoothMaterial.setHorizontalWrapMode(wrapMode);
    markDirty(DirtyMaterial);
}

void DSGRadiusDefaultImageNode::setRadius(qreal radius)
{
    if (radius != m_radius) {
        m_radius = radius;
        m_dirtyGeometry = true;
        m_dirtyRadius = true;
    }
}

void DSGRadiusDefaultImageNode::setTopLeftRadius(qreal radius)
{
    if (radius != m_topLeftRadius)  {
        m_topLeftRadius = radius;
        m_dirtyGeometry = true;
        m_dirtyRadius = true;
    }
}

void DSGRadiusDefaultImageNode::setTopRightRadius(qreal radius)
{
    if (radius != m_topRightRadius) {
        m_topRightRadius = radius;
        m_dirtyGeometry = true;
        m_dirtyRadius = true;
    }
}

void DSGRadiusDefaultImageNode::setBottomLeftRadius(qreal radius)
{
    if (radius != m_bottomLeftRadius) {
        m_bottomLeftRadius = radius;
        m_dirtyGeometry = true;
        m_dirtyRadius = true;
    }
}

void DSGRadiusDefaultImageNode::setBottomRightRadius(qreal radius)
{
    if (radius != m_bottomRightRadius) {
        m_bottomRightRadius = radius;
        m_dirtyGeometry = true;
        m_dirtyRadius = true;
    }
}

void DSGRadiusDefaultImageNode::updateMaterialAntialiasing()
{
    if (m_antialiasing) {
        if (m_radius > 0
            || m_topLeftRadius > 0
            || m_topRightRadius > 0
            || m_bottomLeftRadius > 0
            || m_bottomRightRadius > 0) {
            setMaterial(&m_radiusMaterial);
        } else {
            setMaterial(&m_smoothMaterial);
        }
        
        setOpaqueMaterial(nullptr);
    } else {
        setMaterial(&m_materialO);
        setOpaqueMaterial(&m_material);
    }
}

void DSGRadiusDefaultImageNode::setMaterialTexture(QSGTexture *texture)
{
    m_material.setTexture(texture);
    m_materialO.setTexture(texture);
    m_radiusMaterial.setTexture(texture);
    m_smoothMaterial.setTexture(texture);
}

QSGTexture *DSGRadiusDefaultImageNode::materialTexture() const
{
    return m_material.texture();
}

bool DSGRadiusDefaultImageNode::updateMaterialBlending()
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
    return x == (x & -x);
}

bool DSGRadiusDefaultImageNode::supportsWrap(const QSize &size) const
{
    return isPowerOfTwo(size.width()) && isPowerOfTwo(size.height());
}

DQUICK_END_NAMESPACE
