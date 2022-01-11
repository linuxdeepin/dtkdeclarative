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

#include "drectanglenode.h"

#include <QSGRendererInterface>
#include <QPainter>
#include <QPainterPath>
#include <QQuickWindow>

DQUICK_USE_NAMESPACE
DQUICK_BEGIN_NAMESPACE

const char *CornerColorShader::vertexShader() const
{
    return "uniform highp mat4 qt_Matrix;                      \n"
           "attribute highp vec4 qt_VertexPosition;            \n"
           "attribute highp vec2 qt_VertexTexCoord;            \n"
           "varying highp vec2 qt_TexCoord;                    \n"
           "void main() {                                      \n"
           "    qt_TexCoord = qt_VertexTexCoord;               \n"
           "    gl_Position = qt_Matrix * qt_VertexPosition;   \n"
           "}";
}

const char *CornerColorShader::fragmentShader() const
{
    return "varying highp vec2 qt_TexCoord;                                               \n"
           "uniform vec4 color;                                                           \n"
           "uniform lowp float qt_Opacity;                                                \n"
           "uniform sampler2D qt_Texture;                                                 \n"
           "void main() {                                                                 \n"
           "    gl_FragColor = color * texture2D(qt_Texture, qt_TexCoord).a * qt_Opacity; \n"
           "}";
}

const char * const *CornerColorShader::attributeNames() const
{
    static const char *attributes[] = {
        "qt_VertexPosition",
        "qt_VertexTexCoord",
        nullptr
    };
    return attributes;
}

void CornerColorShader::updateState(const QSGMaterialShader::RenderState &state, QSGMaterial *newMaterial, QSGMaterial *oldMaterial)
{
    QOpenGLShaderProgram * const program = QSGMaterialShader::program();
    CornerColorMaterial *newSurface = static_cast<CornerColorMaterial *>(newMaterial);

    QSGOpaqueTextureMaterialShader::updateState(state, newSurface, oldMaterial);

    if (state.isOpacityDirty()) {
        program->setUniformValue(m_idQtOpacity, state.opacity());
    }

    program->setUniformValue("color", newSurface->color());
}

void CornerColorShader::initialize()
{
    QSGOpaqueTextureMaterialShader::initialize();
    QOpenGLShaderProgram * const program = QSGMaterialShader::program();
    m_idQtOpacity = program->uniformLocation("qt_Opacity");
}

CornerColorMaterial::CornerColorMaterial()
    : QSGOpaqueTextureMaterial()
{
    // blending must be turned on to handle cornor mask
    setFlag(Blending, true);
}

QSGMaterialType *CornerColorMaterial::type() const
{
    static QSGMaterialType type;

    return &type;
}

QSGMaterialShader *CornerColorMaterial::createShader() const
{
    return new CornerColorShader;
}

int CornerColorMaterial::compare(const QSGMaterial *other) const
{
    return QSGOpaqueTextureMaterial::compare(other);
}

DRectangleNode::DRectangleNode()
    : m_geometry(QSGGeometry::defaultAttributes_ColoredPoint2D(), 8)
    , m_cornerGeometry(QSGGeometry::defaultAttributes_TexturedPoint2D(), 0)
{
    setGeometry(&m_geometry);
    setMaterial(&m_material);

    m_cornerNode.setGeometry(&m_cornerGeometry);
    m_cornerNode.setMaterial(&m_cornerMaterial);
    m_cornerGeometry.setDrawingMode(GL_TRIANGLES);
}

void DRectangleNode::setRect(const QRectF &r)
{
    if (m_rect == r)
        return;

    m_geometryChanged |= true;
    m_rect = r;
}

void DRectangleNode::setRadius(qreal radius)
{
    qreal cornorRadius = m_cornerMaterial.radius();
    if (qFuzzyCompare(cornorRadius, radius))
        return;

    m_geometryChanged |= true;
    m_cornerMaterial.setRadius(std::min({rect().width() / 2.0, rect().height() / 2.0, radius}));
    if (radius > 0) {
        if (childCount() == 0) {
            appendChildNode(&m_cornerNode);
            markDirty(QSGNode::DirtyNodeAdded);
        }
    } else {
        if (childCount() > 0) {
            removeChildNode(&m_cornerNode);
            markDirty(QSGNode::DirtyNodeRemoved);
        }
    }
    m_cornerNode.markDirty(DirtyMaterial);
}

void DRectangleNode::setColor(const QColor &color)
{
    if (m_cornerMaterial.color() == color)
        return;

    m_cornerMaterial.setColor(color);
    markDirty(DirtyMaterial);
    m_cornerNode.markDirty(DirtyMaterial);
}

void DRectangleNode::setMakTexture(QSGTexture *texture)
{
    if (m_cornerMaterial.texture() == texture)
        return;

    texture->setFiltering(QSGTexture::Linear);
    texture->setVerticalWrapMode(QSGTexture::ClampToEdge);
    texture->setHorizontalWrapMode(QSGTexture::ClampToEdge);
    if (!texture->hasAlphaChannel())
        qWarning() << "The mask material does not contain an alpha channel";

    m_cornerMaterial.setTexture(texture);
    m_cornerNode.markDirty(DirtyMaterial);
}

void DRectangleNode::setCorners(DQuickRectangle::Corners corners)
{
    if (m_coners == corners)
        return;

    m_geometryChanged |= true;
    m_coners = corners;
}

void DRectangleNode::update()
{
    if (m_geometryChanged) {
        updateGeometry();
        m_geometryChanged = false;
    }
}

void DRectangleNode::updateGeometry()
{
    const QRectF rect = QRectF(0, 0, 1, 1);
    qreal cornorRadius = m_cornerMaterial.radius();
    if (cornorRadius > 0)
    {
        qreal radius = std::min({ m_rect.width() / 2, m_rect.height() / 2, cornorRadius });

        m_geometry.allocate(8);
        const float outerL = m_rect.left();
        const float innerL = m_rect.left() + radius;
        const float innerR = m_rect.right() - radius;
        const float outerR = m_rect.right();

        const float outerT = m_rect.top();
        const float innerT = m_rect.top() + radius;
        const float innerB = m_rect.bottom() - radius;
        const float outerB = m_rect.bottom();

        // inner polygon
        QSGGeometry::ColoredPoint2D *vertices = m_geometry.vertexDataAsColoredPoint2D();

        bool bottomLeft = m_coners.testFlag(DQuickRectangle::BottomLeftCorner);
        bool topLeft = m_coners.testFlag(DQuickRectangle::TopLeftCorner);
        bool bottomRight = m_coners.testFlag(DQuickRectangle::BottomRightCorner);
        bool topRight = m_coners.testFlag(DQuickRectangle::TopRightCorner);
        uchar r = uchar(qRound(color().redF() * color().alphaF() * 255));
        uchar g = g = uchar(qRound(color().greenF() * color().alphaF() * 255));
        uchar b = uchar(qRound(color().blueF() * color().alphaF() * 255));
        uchar a = uchar(qRound(color().alphaF() * 255));
        //          (3)*********(5)
        //            *         *
        //           *           *
        //          *             *
        //         *(1)            *(7)
        //         *               *
        //         *(0)            *
        //          *              *(6)
        //           *            *
        //            *          *
        //          (2)**********(4)
        vertices[0].set(outerL, innerB, r, g, b, a); // Outer left, inner bottom
        vertices[1].set(outerL, innerT, r, g, b, a); // Outer left, inner top
        vertices[2].set(bottomLeft ? innerL : m_rect.left(), outerB, r, g, b, a); // Inner left, outer bottom
        vertices[3].set(topLeft ? innerL : m_rect.left(), outerT, r, g, b, a); // Inner left, outer top
        vertices[4].set(bottomRight ? innerR : m_rect.right(), outerB, r, g, b, a); // Inner right, outer botton
        vertices[5].set(topRight ? innerR : m_rect.right(), outerT, r, g, b, a); // Inner right, outer top
        vertices[6].set(outerR, innerB, r, g, b, a); // Outer right, inner bottom
        vertices[7].set(outerR, innerT, r, g, b, a); // Outer right, inner top

        // four corner triangle
        int num = 0;
        if (bottomLeft)
            num += 3;
        if (topLeft)
            num += 3;
        if (bottomRight)
            num += 3;
        if (topRight)
            num += 3;

        m_cornerGeometry.allocate(num);
        QSGGeometry::TexturedPoint2D *corners = m_cornerGeometry.vertexDataAsTexturedPoint2D();
        int vertexNum = 0;
        // Bottom left
        //      (1)*
        //         **
        //         * *
        //         *   *
        //      (0)******(2)
        if (bottomLeft) {
            corners[0].set(outerL, outerB, 0, 0);
            corners[1].set(outerL, innerB, 0, 1);
            corners[2].set(innerL, outerB, 1, 0);
            vertexNum += 3;
        }

        // Top left
        //      (3)*****(5)
        //         *  *
        //         * *
        //         **
        //         *(4)
        if (topLeft) {
            corners[vertexNum + 0].set(outerL, outerT, 0, 0);
            corners[vertexNum + 1].set(outerL, innerT, 0, 1);
            corners[vertexNum + 2].set(innerL, outerT, 1, 0);
            vertexNum += 3;
        }

        // Bottom right
        //               *(7)
        //              **
        //             * *
        //            *  *
        //        (8)*****(6)
        if (bottomRight) {
            corners[vertexNum + 0].set(outerR, outerB, 0, 0);
            corners[vertexNum + 1].set(outerR, innerB, 0, 1);
            corners[vertexNum + 2].set(innerR, outerB, 1, 0);
            vertexNum += 3;
        }

        // Top right
        //     (11)*****(9)
        //          *  *
        //           * *
        //            **
        //             *(10)
        if (topRight) {
            corners[vertexNum + 0].set(outerR, outerT, 0, 0);
            corners[vertexNum + 1].set(outerR, innerT, 0, 1);
            corners[vertexNum + 2].set(innerR, outerT, 1, 0);
        }

        m_cornerNode.markDirty(DirtyGeometry);
        markDirty(QSGNode::DirtyGeometry);
    } else {
        m_geometry.allocate(4);
        QSGGeometry::updateRectGeometry(&m_geometry, m_rect);
        markDirty(QSGNode::DirtyGeometry);
    }
}

DSoftRectangleNode::DSoftRectangleNode(QQuickItem *owner)
    : m_item(owner)
{
    Q_ASSERT(owner);
    m_window = m_item->window();
}

QSGRenderNode::StateFlags DSoftRectangleNode::changedStates() const
{
    return {};
}

void DSoftRectangleNode::render(const RenderState *state)
{
    Q_ASSERT(m_window);

    QSGRendererInterface *rif = m_window->rendererInterface();
    QPainter *p = static_cast<QPainter *>(rif->getResource(m_window, QSGRendererInterface::PainterResource));
    Q_ASSERT(p);

    const QRegion *clipRegion = state->clipRegion();
    if (clipRegion && !clipRegion->isEmpty())
        p->setClipRegion(*clipRegion, Qt::ReplaceClip);

    p->setTransform(matrix()->toTransform());
    p->setOpacity(inheritedOpacity());

    QPainterPath rectPath;
    p->setPen(Qt::NoPen);
    rectPath.addRect(0, 0, m_item->width() - 1, m_item->height() - 1);

    bool bottomLeft = m_coners.testFlag(DQuickRectangle::BottomLeftCorner);
    bool topLeft = m_coners.testFlag(DQuickRectangle::TopLeftCorner);
    bool bottomRight = m_coners.testFlag(DQuickRectangle::BottomRightCorner);
    bool topRight = m_coners.testFlag(DQuickRectangle::TopRightCorner);

    p->setRenderHint(QPainter::Antialiasing, m_item->antialiasing());
    QPainterPath path;
    qreal radius = std::min({m_item->width() / 2.0, m_item->height() / 2.0, m_radius});

    if (topLeft) {
        path.moveTo(0, 0);
        path.arcTo(0, 0,  radius * 2,  radius * 2, 90, 90);
        path.lineTo(0, 0);
    }

    if (topRight) {
        path.moveTo(m_item->width(), 0);
        QRectF rect1(m_item->width() -  radius * 2, 0,
                     radius * 2,  radius * 2);
        path.arcTo(rect1, 0, 90);
        path.lineTo(m_item->width(), 0);
    }

    if (bottomLeft) {
        path.moveTo(0, m_item->height());
        QRectF rect2(0, m_item->height() -  radius * 2,
                     radius * 2, radius * 2);
        path.arcTo(rect2, 180, 90);
        path.lineTo(0, m_item->height());
    }

    if (bottomRight) {
        path.moveTo(m_item->width(), m_item->height());
        QRectF f2(m_item->width() -  radius * 2,
                  m_item->height() -  radius * 2,
                  radius * 2,  radius * 2);
        path.arcTo(f2, 270, 90);
        path.lineTo(m_item->width(),  m_item->height());
    }

    rectPath = rectPath - path;
    p->fillPath(rectPath, m_color);
}

QSGRenderNode::RenderingFlags DSoftRectangleNode::flags() const
{
    return BoundedRectRendering;
}

QRectF DSoftRectangleNode::rect() const
{
    return QRectF(0, 0, m_item->width(), m_item->height());
}

void DSoftRectangleNode::setRadius(qreal radius)
{
    if (qFuzzyCompare(radius, m_radius))
        return;

    m_radius = radius;
    markDirty(DirtyMaterial);
}

void DSoftRectangleNode::setColor(const QColor &color)
{
    if (color == m_color)
        return;

    m_color = color;
    markDirty(DirtyMaterial);
}

void DSoftRectangleNode::setCorners(DQuickRectangle::Corners corners)
{
    if (corners == m_coners)
        return;

    m_coners = corners;
    markDirty(DirtyMaterial);
}

DQUICK_END_NAMESPACE
