/*
 * Copyright (C) 2021 UnionTech Technology Co., Ltd.
 *
 * Author:     JiDe Zhang <zhangjide@deepin.org>
 *
 * Maintainer: JiDe Zhang <zhangjide@deepin.org>
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
#include "dblurimagenode_p.h"

#if QT_VERSION < QT_VERSION_CHECK(5, 14, 0)
#include <private/qsgtexture_p.h>
#else
#include <private/qsgplaintexture_p.h>
#endif
#include <private/qquickitem_p.h>
#include <private/qmemrotate_p.h>
#include <QPainter>
#ifndef QT_NO_OPENGL
#include <QOpenGLFunctions>
#include <QOpenGLContext>
#include <QOpenGLBuffer>
#endif

DQUICK_BEGIN_NAMESPACE

// Copy from qpixmapfilter.cpp begin
template <int shift>
inline int qt_static_shift(int value)
{
    if (shift == 0)
        return value;
    else if (shift > 0)
        return value << (uint(shift) & 0x1f);
    else
        return value >> (uint(-shift) & 0x1f);
}

template<int aprec, int zprec>
inline void qt_blurinner(uchar *bptr, int &zR, int &zG, int &zB, int &zA, int alpha)
{
    QRgb *pixel = (QRgb *)bptr;

#define Z_MASK (0xff << zprec)
    const int A_zprec = qt_static_shift<zprec - 24>(*pixel) & Z_MASK;
    const int R_zprec = qt_static_shift<zprec - 16>(*pixel) & Z_MASK;
    const int G_zprec = qt_static_shift<zprec - 8>(*pixel)  & Z_MASK;
    const int B_zprec = qt_static_shift<zprec>(*pixel)      & Z_MASK;
#undef Z_MASK

    const int zR_zprec = zR >> aprec;
    const int zG_zprec = zG >> aprec;
    const int zB_zprec = zB >> aprec;
    const int zA_zprec = zA >> aprec;

    zR += alpha * (R_zprec - zR_zprec);
    zG += alpha * (G_zprec - zG_zprec);
    zB += alpha * (B_zprec - zB_zprec);
    zA += alpha * (A_zprec - zA_zprec);

#define ZA_MASK (0xff << (zprec + aprec))
    *pixel =
        qt_static_shift<24 - zprec - aprec>(zA & ZA_MASK)
        | qt_static_shift<16 - zprec - aprec>(zR & ZA_MASK)
        | qt_static_shift<8 - zprec - aprec>(zG & ZA_MASK)
        | qt_static_shift<-zprec - aprec>(zB & ZA_MASK);
#undef ZA_MASK
}

const static int alphaIndex = (QSysInfo::ByteOrder == QSysInfo::BigEndian ? 0 : 3);

template<int aprec, int zprec>
inline void qt_blurinner_alphaOnly(uchar *bptr, int &z, int alpha)
{
    const int A_zprec = int(*(bptr)) << zprec;
    const int z_zprec = z >> aprec;
    z += alpha * (A_zprec - z_zprec);
    *(bptr) = z >> (zprec + aprec);
}

template<int aprec, int zprec, bool alphaOnly>
inline void qt_blurrow(QImage & im, int line, int alpha)
{
    uchar *bptr = im.scanLine(line);

    int zR = 0, zG = 0, zB = 0, zA = 0;

    if (alphaOnly && im.format() != QImage::Format_Indexed8)
        bptr += alphaIndex;

    const int stride = im.depth() >> 3;
    const int im_width = im.width();
    for (int index = 0; index < im_width; ++index) {
        if (alphaOnly)
            qt_blurinner_alphaOnly<aprec, zprec>(bptr, zA, alpha);
        else
            qt_blurinner<aprec, zprec>(bptr, zR, zG, zB, zA, alpha);
        bptr += stride;
    }

    bptr -= stride;

    for (int index = im_width - 2; index >= 0; --index) {
        bptr -= stride;
        if (alphaOnly)
            qt_blurinner_alphaOnly<aprec, zprec>(bptr, zA, alpha);
        else
            qt_blurinner<aprec, zprec>(bptr, zR, zG, zB, zA, alpha);
    }
}

/*
*  expblur(QImage &img, int radius)
*
*  Based on exponential blur algorithm by Jani Huhtanen
*
*  In-place blur of image 'img' with kernel
*  of approximate radius 'radius'.
*
*  Blurs with two sided exponential impulse
*  response.
*
*  aprec = precision of alpha parameter
*  in fixed-point format 0.aprec
*
*  zprec = precision of state parameters
*  zR,zG,zB and zA in fp format 8.zprec
*/
template <int aprec, int zprec, bool alphaOnly>
static void expblur(QImage &img, qreal radius, bool improvedQuality = false, int transposed = 0)
{
    // halve the radius if we're using two passes
    if (improvedQuality)
        radius *= qreal(0.5);

    Q_ASSERT(img.format() == QImage::Format_ARGB32_Premultiplied
             || img.format() == QImage::Format_RGB32
             || img.format() == QImage::Format_Indexed8
             || img.format() == QImage::Format_Grayscale8);

    // choose the alpha such that pixels at radius distance from a fully
    // saturated pixel will have an alpha component of no greater than
    // the cutOffIntensity
    const qreal cutOffIntensity = 2;
    int alpha = radius <= qreal(1e-5)
        ? ((1 << aprec)-1)
        : qRound((1<<aprec)*(1 - qPow(cutOffIntensity * (1 / qreal(255)), 1 / radius)));

    int img_height = img.height();
    for (int row = 0; row < img_height; ++row) {
        for (int i = 0; i <= int(improvedQuality); ++i)
            qt_blurrow<aprec, zprec, alphaOnly>(img, row, alpha);
    }

    QImage temp(img.height(), img.width(), img.format());
    temp.setDevicePixelRatio(img.devicePixelRatioF());
    if (transposed >= 0) {
        if (img.depth() == 8) {
            qt_memrotate270(reinterpret_cast<const quint8*>(img.bits()),
                            img.width(), img.height(), img.bytesPerLine(),
                            reinterpret_cast<quint8*>(temp.bits()),
                            temp.bytesPerLine());
        } else {
            qt_memrotate270(reinterpret_cast<const quint32*>(img.bits()),
                            img.width(), img.height(), img.bytesPerLine(),
                            reinterpret_cast<quint32*>(temp.bits()),
                            temp.bytesPerLine());
        }
    } else {
        if (img.depth() == 8) {
            qt_memrotate90(reinterpret_cast<const quint8*>(img.bits()),
                           img.width(), img.height(), img.bytesPerLine(),
                           reinterpret_cast<quint8*>(temp.bits()),
                           temp.bytesPerLine());
        } else {
            qt_memrotate90(reinterpret_cast<const quint32*>(img.bits()),
                           img.width(), img.height(), img.bytesPerLine(),
                           reinterpret_cast<quint32*>(temp.bits()),
                           temp.bytesPerLine());
        }
    }

    img_height = temp.height();
    for (int row = 0; row < img_height; ++row) {
        for (int i = 0; i <= int(improvedQuality); ++i)
            qt_blurrow<aprec, zprec, alphaOnly>(temp, row, alpha);
    }

    if (transposed == 0) {
        if (img.depth() == 8) {
            qt_memrotate90(reinterpret_cast<const quint8*>(temp.bits()),
                           temp.width(), temp.height(), temp.bytesPerLine(),
                           reinterpret_cast<quint8*>(img.bits()),
                           img.bytesPerLine());
        } else {
            qt_memrotate90(reinterpret_cast<const quint32*>(temp.bits()),
                           temp.width(), temp.height(), temp.bytesPerLine(),
                           reinterpret_cast<quint32*>(img.bits()),
                           img.bytesPerLine());
        }
    } else {
        img = temp;
    }
}

static inline void qt_blurImage(QImage &blurImage, qreal radius, bool quality, int transposed = 0)
{
    if (blurImage.format() == QImage::Format_Indexed8 || blurImage.format() == QImage::Format_Grayscale8)
        expblur<12, 10, true>(blurImage, radius, quality, transposed);
    else
        expblur<12, 10, false>(blurImage, radius, quality, transposed);
}
// Copy from qpixmapfilter.cpp end

DSoftwareBlurImageNode::DSoftwareBlurImageNode(QQuickItem *owner)
    : item(owner)
{

}

void DSoftwareBlurImageNode::setTexture(QSGTexture *texture)
{
    if (m_texture == texture)
        return;
    m_texture = texture;
    DSoftwareBlurImageNode::releaseResources();
    markDirty(DirtyMaterial);
}

void DSoftwareBlurImageNode::setRadius(qreal radius)
{
    if (qFuzzyCompare(radius, this->radius))
        return;
    this->radius = radius;
    markDirty(DirtyMaterial);
}

void DSoftwareBlurImageNode::setSourceRect(const QRectF &source)
{
    if (this->sourceRect == source)
        return;
    this->sourceRect = source;
    markDirty(DirtyMaterial);
}

void DSoftwareBlurImageNode::setRect(const QRectF &target)
{
    if (this->targetRect == target)
        return;
    this->targetRect = target;
    markDirty(DirtyMaterial);
}

void DSoftwareBlurImageNode::setDisabledOpaqueRendering(bool disabled)
{
    if (this->disabledOpaqueRendering == disabled)
        return;
    this->disabledOpaqueRendering = disabled;
    markDirty(DirtyForceUpdate);
}

void DSoftwareBlurImageNode::setBlendColor(const QColor &color)
{
    if (this->blendColor == color)
        return;
    this->blendColor = color;
    markDirty(DirtyMaterial);
}

void DSoftwareBlurImageNode::setFollowMatrixForSource(bool on)
{
    if (followMatrixForSource == on)
        return;
    followMatrixForSource = on;
    markDirty(DirtyMaterial);
}

static inline QImage refQImage(QImage &source, const QRectF &rect) {
    uchar *offset = source.bits() + qRound(rect.x() + rect.y() * source.bytesPerLine());
    QImage image(offset, rect.width(), rect.height(), source.bytesPerLine(), source.format());
    image.setDevicePixelRatio(source.devicePixelRatio());
    return image;
}

void DSoftwareBlurImageNode::render(const RenderState *state)
{
    if (!sourceRect.isValid() || !m_texture)
        return;

    updateCachedImage();

    if (cachedSource.isNull())
        return;

    QSGRendererInterface *rif = item->window()->rendererInterface();
    QPainter *p = static_cast<QPainter *>(rif->getResource(item->window(),
                                                           QSGRendererInterface::PainterResource));
    Q_ASSERT(p);

    const QRegion *clipRegion = state->clipRegion();
    if (clipRegion && !clipRegion->isEmpty())
        p->setClipRegion(*clipRegion, Qt::ReplaceClip); // must be done before setTransform

    p->setOpacity(inheritedOpacity());

    const qreal dpr = cachedSource.devicePixelRatio();
    QPointF offset = QPointF(sourceRect.x() * dpr, sourceRect.y() * dpr);
    if (m_texture->isAtlasTexture()) {
        const QRectF subRect = m_texture->normalizedTextureSubRect();
        offset.rx() += subRect.x() * cachedSource.width();
        offset.ry() += subRect.y() * cachedSource.height();
    }
    QRectF actualSourceRect(offset.x(), offset.y(),
                            sourceRect.width() * dpr,
                            sourceRect.height() * dpr);

    const QTransform t = matrix()->toTransform();
    QRectF actualTargetRect = targetRect;
    if (followMatrixForSource) {
        // map the targetRect to the actualSourceRect
        QPainterPath path;
        path.addRect(targetRect);
        path = t.map(path);
        p->setClipPath(path, Qt::IntersectClip);

        actualSourceRect.setSize(t.mapRect(actualSourceRect).size());
        actualTargetRect = t.mapRect(targetRect);
    } else {
        p->setTransform(t);
    }

    // do blur
    QImage sourceRef = refQImage(cachedSource, actualSourceRect);
    // TODO: Don't transparent the borders
    qt_blurImage(sourceRef, radius, false);
    p->drawImage(actualTargetRect, cachedSource, actualSourceRect);
    if (blendColor.isValid()) {
        p->fillRect(actualTargetRect, blendColor);
    }
}

void DSoftwareBlurImageNode::releaseResources()
{
    static QImage globalNullImage;
    cachedSource = globalNullImage;
}

QSGRenderNode::RenderingFlags DSoftwareBlurImageNode::flags() const
{
    RenderingFlags rf = BoundedRectRendering;
    if (!disabledOpaqueRendering &&( !m_texture || !m_texture->hasAlphaChannel()))
        rf |= OpaqueRendering;
    return rf;
}

QRectF DSoftwareBlurImageNode::rect() const
{
    return targetRect;
}

void DSoftwareBlurImageNode::updateCachedImage()
{
    Q_ASSERT(m_texture);
    if (auto image = qobject_cast<QSGPlainTexture*>(m_texture)) {
        cachedSource = image->image();
    } else if (QSGLayer *pt = qobject_cast<QSGLayer *>(m_texture)) {
        cachedSource = pt->toImage();
    } else {
        DSoftwareBlurImageNode::releaseResources();
    }
}

#ifndef QT_NO_OPENGL

DBlurEffectNode::DBlurEffectNode(QQuickItem *owner)
    : QSGRenderNode ()
    , m_item(owner)
{
}

DBlurEffectNode::~DBlurEffectNode()
{
    delete m_programKawaseUp;
    m_programKawaseUp = nullptr;
    delete m_programKawaseDown;
    m_programKawaseDown = nullptr;
    delete m_program;
    m_program = nullptr;

    qDeleteAll(m_fboVector);
    m_fboVector.clear();

    delete m_vbo;
    m_vbo = nullptr;

    delete  m_sampleVbo;
    m_sampleVbo = nullptr;
}

void DBlurEffectNode::setTexture(QSGTexture *texture)
{
    if (m_texture == texture)
        return;

    m_texture = texture;
    m_needUpdateFBO = true;
    markDirty(DirtyMaterial);
}

void DBlurEffectNode::setRadius(qreal radius)
{
    // TODO(xiaoyaobing): I don't want to do this, but the radius of software rendering and
    //                    hardware rendering can't be unified
    if (qRound(radius / 10) == m_radius)
        return;

    this->m_radius = qRound(radius / 10);
    m_needUpdateFBO = true;
    markDirty(DirtyMaterial);
}

void DBlurEffectNode::setSourceRect(const QRectF &source)
{
    if (m_sourceRect == source)
        return;

    m_sourceRect = source;
    markDirty(DirtyMaterial);
}

void DBlurEffectNode::setRect(const QRectF &target)
{
    if (m_targetRect == target)
        return;

    m_targetRect = target;
    markDirty(DirtyMaterial);
}

void DBlurEffectNode::setDisabledOpaqueRendering(bool disabled)
{
    if (m_disabledOpaqueRendering == disabled)
        return;

    m_disabledOpaqueRendering = disabled;
    markDirty(DirtyForceUpdate);
}

void DBlurEffectNode::setBlendColor(const QColor &color)
{
    if (m_blendColor == color)
        return;

    m_blendColor = color;

    markDirty(DirtyMaterial);
}

// TODO(xiaoyaobing): Not implemented yet
void DBlurEffectNode::setFollowMatrixForSource(bool on)
{
    if (m_followMatrixForSource == on)
        return;

    m_followMatrixForSource = on;

    markDirty(DirtyMaterial);
}

void DBlurEffectNode::render(const QSGRenderNode::RenderState *state)
{
    if (!m_sourceRect.isValid() || !m_texture)
        return;

    const bool needsWrap = QSGRendererInterface::isApiRhiBased(m_item->window()->rendererInterface()->graphicsApi());
    if (Q_LIKELY(needsWrap)) {
        m_item->window()->beginExternalCommands();
        m_item->window()->resetOpenGLState();
    }

    if (Q_LIKELY(!m_programKawaseUp))
        initialize();

    if (Q_LIKELY(m_needUpdateFBO)) {
        initFBOTextures();
        m_needUpdateFBO = false;
    }

    if (Q_LIKELY(m_fboVector.count() > 1)) {
        applyDaulBlur(m_fboVector[1], m_texture->textureId(), m_programKawaseDown, state,
                m_matrixKawaseDownUniform, 2);

        for (int i = 1; i < m_radius; i++) {
            applyDaulBlur(m_fboVector[i + 1], m_fboVector[i]->texture(), m_programKawaseDown, state,
                    m_matrixKawaseDownUniform, qPow(2, i + 1));
        }

        for (int i = m_radius; i > 0; i--) {
            applyDaulBlur(m_fboVector[i - 1], m_fboVector[i]->texture(), m_programKawaseUp, state,
                    m_matrixKawaseUpUniform, qPow(2, i - 1));
        }

        if (Q_LIKELY(m_fboVector.count() > 0))
            renderToScreen(m_fboVector[0]->texture(), state);
    }

    m_item->window()->resetOpenGLState();

    if (Q_LIKELY(needsWrap))
        m_item->window()->endExternalCommands();
}

QSGRenderNode::StateFlags DBlurEffectNode::changedStates() const
{
    return BlendState | ScissorState;
}

QSGRenderNode::RenderingFlags DBlurEffectNode::flags() const
{
    RenderingFlags rf = BoundedRectRendering;
    if (!m_disabledOpaqueRendering &&( !m_texture || !m_texture->hasAlphaChannel()))
        rf |= OpaqueRendering;
    return rf;
}

QRectF DBlurEffectNode::rect() const
{
    return m_targetRect;
}

void DBlurEffectNode::initialize()
{
    initDispalyShader();
    initBlurSahder();
}

void DBlurEffectNode::initBlurSahder()
{
    m_programKawaseUp = new QOpenGLShaderProgram;
    m_programKawaseDown = new QOpenGLShaderProgram;

    m_programKawaseUp->addCacheableShaderFromSourceFile(QOpenGLShader::Vertex,
                                                        ":/dtk/declarative/shaders/dualkawaseup.vert");
    m_programKawaseUp->addCacheableShaderFromSourceFile(QOpenGLShader::Fragment,
                                                        ":/dtk/declarative/shaders/dualkawaseup.frag");
    m_programKawaseUp->bindAttributeLocation("posAttr", 0);
    m_programKawaseUp->bindAttributeLocation("qt_VertexTexCoord", 1);
    m_programKawaseUp->link();

    m_matrixKawaseUpUniform = m_programKawaseUp->uniformLocation("matrix");

    m_programKawaseDown->addCacheableShaderFromSourceFile(QOpenGLShader::Vertex,
                                                          ":/dtk/declarative/shaders/dualkawasedown.vert");
    m_programKawaseDown->addCacheableShaderFromSourceFile(QOpenGLShader::Fragment,
                                                          ":/dtk/declarative/shaders/dualkawasedown.frag");
    m_programKawaseDown->bindAttributeLocation("posAttr", 0);
    m_programKawaseDown->bindAttributeLocation("qt_VertexTexCoord", 1);
    m_programKawaseDown->link();

    m_matrixKawaseDownUniform = m_programKawaseDown->uniformLocation("matrix");

    const int VERTEX_SIZE = 8 * sizeof(GLfloat);

    static GLfloat texCoord[] = {
        0.0f, 0.0f,
        0.0f, 1.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
    };

    m_sampleVbo = new QOpenGLBuffer;
    m_sampleVbo->create();
    m_sampleVbo->bind();
    m_sampleVbo->allocate(VERTEX_SIZE + sizeof(texCoord));
    m_sampleVbo->write(VERTEX_SIZE, texCoord, sizeof(texCoord));
}

void DBlurEffectNode::applyDaulBlur(QOpenGLFramebufferObject *targetFBO, GLuint sourceTexture, QOpenGLShaderProgram *shader
                                  , const QSGRenderNode::RenderState *state, int matrixUniform, int scale)
{
    targetFBO->bind();
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
    shader->bind();

    // TODO(xiaoyaobing): Shader pixel offset value during hardware rendering, because software rendering
    //                    does not, so write a fixed value here
    shader->setUniformValue("offset", QVector2D(8, 8));
    shader->setUniformValue("iResolution", QVector2D(targetFBO->size().width(), targetFBO->size().height()));
    shader->setUniformValue("halfpixel", QVector2D(0.5 / targetFBO->size().width(), 0.5 / targetFBO->size().height()));
    float yOffset = m_item->window()->height() - qRound(m_sourceRect.height() / scale);
    shader->setUniformValue(matrixUniform, *state->projectionMatrix() * QMatrix4x4(1, 0, 0, 0,
                                                                                   0, 1, 0, yOffset,
                                                                                   0, 0, 1, 0,
                                                                                   0, 0, 0, 1));
    m_sampleVbo->bind();
    QPointF p0(0, 0);
    QPointF p1(0, qRound(m_sourceRect.height() / scale));
    QPointF p2(qRound(m_sourceRect.width() / scale), 0);
    QPointF p3(qRound(m_sourceRect.width() / scale), qRound(m_sourceRect.height() / scale));

    GLfloat vertices[8] = { GLfloat(p0.x()), GLfloat(p0.y()),
                            GLfloat(p1.x()), GLfloat(p1.y()),
                            GLfloat(p2.x()), GLfloat(p2.y()),
                            GLfloat(p3.x()), GLfloat(p3.y()) };

    m_sampleVbo->write(0, vertices, sizeof(vertices));

    shader->setAttributeBuffer(0, GL_FLOAT, 0, 2);
    shader->setAttributeBuffer(1, GL_FLOAT, sizeof(vertices), 2);
    shader->enableAttributeArray(0);
    shader->enableAttributeArray(1);
    m_sampleVbo->release();

    glEnable(GL_TEXTURE_2D);
    f->glBindTexture(GL_TEXTURE_2D, sourceTexture);
    f->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    f->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    f->glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glDisable(GL_TEXTURE_2D);
    shader->release();
    targetFBO->release();
}

void DBlurEffectNode::renderToScreen(GLuint sourceTexture, const QSGRenderNode::RenderState *state)
{
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
    f->glBindTexture(GL_TEXTURE_2D, sourceTexture);
    f->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    f->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    QOpenGLFramebufferObject::bindDefault();
    m_program->bind();
    m_program->setUniformValue(m_matrixUniform, *state->projectionMatrix() * *matrix());
    m_program->setUniformValue(m_opacityUniform, float(inheritedOpacity()));
    m_program->setUniformValue("blendColor", m_blendColor);

    m_vbo->bind();

    QPointF p0(0, 0);
    QPointF p1(0, m_sourceRect.height());
    QPointF p2(m_sourceRect.width(), 0);
    QPointF p3(m_sourceRect.width(), m_sourceRect.height());

    GLfloat vertices[8] = { GLfloat(p0.x()), GLfloat(p0.y()),
                            GLfloat(p1.x()), GLfloat(p1.y()),
                            GLfloat(p2.x()), GLfloat(p2.y()),
                            GLfloat(p3.x()), GLfloat(p3.y()) };

    m_vbo->write(0, vertices, sizeof(vertices));

    m_program->setAttributeBuffer(0, GL_FLOAT, 0, 2);
    m_program->setAttributeBuffer(1, GL_FLOAT, sizeof(vertices), 2);
    m_program->enableAttributeArray(0);
    m_program->enableAttributeArray(1);
    m_vbo->release();

    f->glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);

    f->glEnable(GL_BLEND);
    f->glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

    if (state->scissorEnabled()) {
        f->glEnable(GL_SCISSOR_TEST);
        const QRect r = state->scissorRect();
        f->glScissor(r.x(), r.y(), r.width(), r.height());
    }
    if (state->stencilEnabled()) {
        f->glEnable(GL_STENCIL_TEST);
        f->glStencilFunc(GL_EQUAL, state->stencilValue(), 0xFF);
        f->glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
    }
    f->glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    markDirty(QSGNode::DirtyGeometry);
    m_program->release();
}

void DBlurEffectNode::initFBOTextures()
{
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
    for (int i = 0; i < m_fboVector.size(); i++) {
        delete m_fboVector[i];
    }

    m_fboVector.clear();
    m_fboVector.append(new QOpenGLFramebufferObject(m_sourceRect.size().toSize(),
                                                    QOpenGLFramebufferObject::CombinedDepthStencil, GL_TEXTURE_2D));
    for (int i = 1; i <= m_radius; i++) {
        m_fboVector.append(new QOpenGLFramebufferObject(m_sourceRect.size().toSize() / qPow(2, i),
                                                        QOpenGLFramebufferObject::CombinedDepthStencil, GL_TEXTURE_2D));

        f->glBindTexture(GL_TEXTURE_2D, m_fboVector.last()->texture());
        f->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        f->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }
}

void DBlurEffectNode::initDispalyShader()
{
    m_program = new QOpenGLShaderProgram;

    static const char *vertexShaderSource =
            "attribute highp vec4 posAttr;              \n"
            "attribute highp vec2 qt_VertexTexCoord;    \n"
            "varying highp vec2 qt_TexCoord;            \n"
            "uniform highp mat4 matrix;                 \n"
            "void main() {                              \n"
            "   qt_TexCoord = qt_VertexTexCoord;        \n"
            "   gl_Position = matrix * posAttr;         \n"
            "}\n";

    static const char *fragmentShaderSource =
            "varying highp vec2 qt_TexCoord;                                                           \n"
            "uniform lowp float opacity;                                                               \n"
            "uniform sampler2D qt_Texture;                                                             \n"
            "uniform highp vec4 blendColor;                                                            \n"
            "void main() {                                                                             \n"
            "   highp vec4 color = texture2D(qt_Texture, qt_TexCoord);                                 \n"
            "   gl_FragColor = (color + blendColor * blendColor.a) * opacity;\n"
            "}\n";

    m_program->addCacheableShaderFromSourceCode(QOpenGLShader::Vertex, vertexShaderSource);
    m_program->addCacheableShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShaderSource);
    m_program->bindAttributeLocation("posAttr", 0);
    m_program->bindAttributeLocation("qt_VertexTexCoord", 1);
    m_program->link();

    m_matrixUniform = m_program->uniformLocation("matrix");
    m_opacityUniform = m_program->uniformLocation("opacity");

    const int VERTEX_SIZE = 8 * sizeof(GLfloat);

    static GLfloat texCoord[] = {
        0.0f, 0.0f,
        0.0f, 1.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
    };

    m_vbo = new QOpenGLBuffer;
    m_vbo->create();
    m_vbo->bind();
    m_vbo->allocate(VERTEX_SIZE + sizeof(texCoord));
    m_vbo->write(VERTEX_SIZE, texCoord, sizeof(texCoord));
}

#endif

DQUICK_END_NAMESPACE
