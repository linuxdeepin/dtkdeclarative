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

#include "qsgprogressbartexture.h"

class OpaqueRectangleVerticalShader : public QSGMaterialShader
{
public:
    const char *vertexShader() const override;
    const char *fragmentShader() const override;
    char const *const *attributeNames() const override;
    void updateState(const RenderState &state, QSGMaterial *newMaterial, QSGMaterial *oldMaterial) override;
    void initialize() override;

private:
    int m_idQtMatrix = -1;
    int m_qtTexture = -1;
};

class RectangleVerticalShader : public OpaqueRectangleVerticalShader
{
public:
    const char *fragmentShader() const override;
    void updateState(const RenderState &state, QSGMaterial *newMaterial, QSGMaterial *oldMaterial) override;
    void initialize() override;

private:
    int m_idQtOpacity = -1;
};

class RectangleCornerShader : public RectangleVerticalShader
{
public:
    const char *vertexShader() const override;
    const char *fragmentShader() const override;
    char const *const *attributeNames() const override;
    void updateState(const RenderState &state, QSGMaterial *newMaterial, QSGMaterial *oldMaterial) override;
    void initialize() override;

private:
    int m_idRadius;
};

const char *OpaqueRectangleVerticalShader::vertexShader() const
{
    return "uniform highp mat4 qt_Matrix;                      \n"
           "attribute highp vec4 qt_VertexPosition;            \n"
           "attribute highp vec2 qt_VertexTexCoord;            \n"
           "varying highp vec2 qt_TexCoord;                    \n"
           "void main() {                                      \n"
           "    qt_TexCoord = qt_VertexTexCoord;\n"
           "    gl_Position = qt_Matrix * qt_VertexPosition;   \n"
           "}";
}

const char *OpaqueRectangleVerticalShader::fragmentShader() const
{
    return "varying highp vec2 qt_TexCoord;                   \n"
           "uniform lowp vec4 color;                           \n"
           "uniform lowp float value;                          \n"
           "uniform highp vec2 direct;                         \n"
           "void main() {                                      \n"
           "    gl_FragColor = color * step(value, dot(qt_TexCoord,direct)); \n"
           "}";
}

const char * const *OpaqueRectangleVerticalShader::attributeNames() const
{
    static const char *attributes[] = {
        "qt_VertexPosition",
        "qt_VertexTexCoord",
        nullptr
    };
    return attributes;
}

void OpaqueRectangleVerticalShader::updateState(const QSGMaterialShader::RenderState &state, QSGMaterial *newMaterial, QSGMaterial *oldMaterial)
{
    QOpenGLShaderProgram * const program = QSGMaterialShader::program();
    OpaqueRectangleTextureMaterial *newSurface = static_cast<OpaqueRectangleTextureMaterial *>(newMaterial);

    program->setUniformValue("color", newSurface->color());
    program->setUniformValue("value", (GLfloat)newSurface->value());
    program->setUniformValue("direct", newSurface->direct());

    if (state.isMatrixDirty()) {
        program->setUniformValue(m_idQtMatrix, state.combinedMatrix());
    }
}

void OpaqueRectangleVerticalShader::initialize()
{
    QOpenGLShaderProgram * const program = QSGMaterialShader::program();

    m_idQtMatrix = program->uniformLocation("qt_Matrix");
    m_qtTexture = program->uniformLocation("qt_Texture");
}

const char *RectangleVerticalShader::fragmentShader() const
{
    return "varying highp vec2 qt_TexCoord;                   \n"
           "uniform lowp vec4 color;                          \n"
           "uniform lowp float value;                         \n"
           "uniform highp vec2 direct;                        \n"
           "uniform lowp float qt_Opacity;                     \n"
           "void main() {                                      \n"
           "    gl_FragColor = color * step(value, dot(qt_TexCoord, direct)) * qt_Opacity;  \n"
           "}";
}

void RectangleVerticalShader::updateState(const QSGMaterialShader::RenderState &state, QSGMaterial *newMaterial, QSGMaterial *oldMaterial)
{
    QOpenGLShaderProgram * const program = QSGMaterialShader::program();
    RectangleTextureMaterial *newSurface = static_cast<RectangleTextureMaterial *>(newMaterial);
    RectangleTextureMaterial *oldSurface = static_cast<RectangleTextureMaterial *>(oldMaterial);

    OpaqueRectangleVerticalShader::updateState(state,&newSurface->opaqueMaterial,oldSurface ? &oldSurface->opaqueMaterial : nullptr);

    if (state.isOpacityDirty()) {
        program->setUniformValue(m_idQtOpacity, state.opacity());
    }
}

void RectangleVerticalShader::initialize()
{
    OpaqueRectangleVerticalShader::initialize();
    QOpenGLShaderProgram * const program = QSGMaterialShader::program();
    m_idQtOpacity = program->uniformLocation("qt_Opacity");
}

const char *RectangleCornerShader::vertexShader() const
{
    return "uniform highp mat4 qt_Matrix;                      \n"
           "attribute highp vec4 qt_VertexPosition;            \n"
           "attribute highp vec2 qt_VertexTexCoord;            \n"
           "attribute highp vec2 vertexCorner;                 \n"
           "varying highp vec2 qt_TexCoord;                    \n"
           "varying highp vec2 corner;                         \n"
           "void main() {                                      \n"
           "    qt_TexCoord = qt_VertexTexCoord;               \n"
           "    gl_Position = qt_Matrix * qt_VertexPosition;   \n"
           "    corner = vertexCorner;                         \n"
           "}";
}

const char *RectangleCornerShader::fragmentShader() const
{
    return "varying highp vec2 qt_TexCoord;                    \n"
           "varying highp vec2 corner;                         \n"
           "uniform vec4 color;                                 \n"
           "uniform float value;                                \n"
           "uniform lowp float qt_Opacity;                     \n"
           "uniform lowp vec2 radius;                          \n"
           "uniform highp vec2 direct;                                 \n"
           "void main() {                                      \n"
           "    gl_FragColor = color * step(value, dot(qt_TexCoord, direct))\n"
           "                 * smoothstep(radius.x, radius.y, dot(corner, corner))\n"
           "                 * qt_Opacity;                     \n"
           "}";
}

const char * const *RectangleCornerShader::attributeNames() const
{
    static const char *attributes[] = {
        "qt_VertexPosition",
        "qt_VertexTexCoord",
        "vertexCorner",
        nullptr
    };
    return attributes;
}

void RectangleCornerShader::updateState(const QSGMaterialShader::RenderState &state, QSGMaterial *newMaterial, QSGMaterial *oldMaterial)
{
    RectangleVerticalShader::updateState(state, newMaterial, oldMaterial);

    QOpenGLShaderProgram * const program = QSGMaterialShader::program();
    CornerRectangleTextureMaterial *newSurface = static_cast<CornerRectangleTextureMaterial *>(newMaterial);

    const float radius = newSurface->radius();
    program->setUniformValue(m_idRadius, (radius + 0.5f) * (radius + 0.5f), (radius - 0.5f) * (radius - 0.5f));
}

void RectangleCornerShader::initialize()
{
    RectangleVerticalShader::initialize();
    QOpenGLShaderProgram * const program = QSGMaterialShader::program();
    m_idRadius = program->uniformLocation("radius");
}


QSGMaterialType *OpaqueRectangleTextureMaterial::type() const
{
    static QSGMaterialType type;

    return &type;
}

QSGMaterialShader *OpaqueRectangleTextureMaterial::createShader() const
{
    return  new OpaqueRectangleVerticalShader;
}

int OpaqueRectangleTextureMaterial::compare(const QSGMaterial *other) const
{
    return 1;
}

QSGMaterialType *RectangleTextureMaterial::type() const
{
    static QSGMaterialType type;

    return &type;
}

QSGMaterialShader *RectangleTextureMaterial::createShader() const
{
    return new RectangleVerticalShader;
}

int RectangleTextureMaterial::compare(const QSGMaterial *other) const
{
    return opaqueMaterial.compare(&static_cast<const RectangleTextureMaterial *>(other)->opaqueMaterial);
}

QSGMaterialType *CornerRectangleTextureMaterial::type() const
{
    static QSGMaterialType type;

    return &type;
}

QSGMaterialShader *CornerRectangleTextureMaterial::createShader() const
{
    return new RectangleCornerShader;
}

int CornerRectangleTextureMaterial::compare(const QSGMaterial *other) const
{
    const CornerRectangleTextureMaterial * const surface = static_cast<const CornerRectangleTextureMaterial *>(other);

    const int result = RectangleTextureMaterial::compare(other);
    if (result != 0) {
        return result;
    } else if (m_radius < surface->m_radius) {
        return -1;
    } else if (m_radius > surface->m_radius) {
        return 1;
    } else {
        return 0;
    }
}
