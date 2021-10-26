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

#include "qsgradiusimagetexture.h"

#include <private/qsgtexture_p.h>
#include <private/qsgtexturematerial_p.h>

#include <QOpenGLContext>
#include <QOpenGLFunctions>

class OpaqueRaiusImageShader : public QSGOpaqueTextureMaterialShader
{
public:
    const char *vertexShader() const override;
    const char *fragmentShader() const override;
    void updateState(const RenderState &state, QSGMaterial *newMaterial, QSGMaterial *oldMaterial) override;
};

class RaiusImageShader : public OpaqueRaiusImageShader
{
public:
    const char *fragmentShader() const override;
    void updateState(const RenderState &state, QSGMaterial *newMaterial, QSGMaterial *oldMaterial) override;
    void initialize() override;

private:
    int m_opacityId;
};

class RaiusImageCornerShader : public RaiusImageShader
{
public:
    const char *vertexShader() const override;
    const char *fragmentShader() const override;
    char const *const *attributeNames() const override;
    void updateState(const RenderState &state, QSGMaterial *newMaterial, QSGMaterial *oldMaterial) override;
    void initialize() override;

private:
    int m_radiusId;
};

const char *OpaqueRaiusImageShader::vertexShader() const
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

const char *OpaqueRaiusImageShader::fragmentShader() const
{
    return "varying highp vec2 qt_TexCoord;                    \n"
           "uniform lowp vec4 specifyColor;                    \n"
           "uniform highp vec2 blendVec;                       \n"
           "uniform sampler2D qt_Texture;                      \n"
           "void main() {                                      \n"
           "    lowp vec4 color = texture2D(qt_Texture, qt_TexCoord);              \n"
           "    gl_FragColor = vec4(color.xyz * blendVec.x + specifyColor.xyz * blendVec.y, color.w); \n"
           "}";
}

void OpaqueRaiusImageShader::updateState(const QSGMaterialShader::RenderState &state, QSGMaterial *newMaterial, QSGMaterial *oldMaterial)
{
    QOpenGLShaderProgram * const program = QSGMaterialShader::program();
    QSGRadiusImageOpaqueTextureMaterial *newTx = static_cast<QSGRadiusImageOpaqueTextureMaterial *>(newMaterial);
    QSGRadiusImageOpaqueTextureMaterial *oldTx= static_cast<QSGRadiusImageOpaqueTextureMaterial *>(oldMaterial);

    QSGOpaqueTextureMaterialShader::updateState(state, newTx, oldTx);

    program->setUniformValue("specifyColor", newTx->specifyColor());
    program->setUniformValue("blendVec", newTx->blendVec());
}

const char *RaiusImageShader::fragmentShader() const
{
    return "varying highp vec2 qt_TexCoord;                    \n"
           "uniform lowp vec4 specifyColor;                    \n"
           "uniform highp vec2 blendVec;                       \n"
           "uniform lowp float qt_Opacity;                     \n"
           "uniform sampler2D qt_Texture;                      \n"
           "void main() {                                      \n"
           "    lowp vec4 color = texture2D(qt_Texture, qt_TexCoord);              \n"
           "    gl_FragColor = vec4(color.xyz * blendVec.x + specifyColor.xyz * blendVec.y, color.w) * qt_Opacity; \n"
           "}";
}

void RaiusImageShader::updateState(const QSGMaterialShader::RenderState &state, QSGMaterial *newMaterial, QSGMaterial *oldMaterial)
{
    QOpenGLShaderProgram * const program = QSGMaterialShader::program();
    QSGRadiusImageTextureMaterial *newTx = static_cast<QSGRadiusImageTextureMaterial *>(newMaterial);
    QSGRadiusImageTextureMaterial *oldTx = static_cast<QSGRadiusImageTextureMaterial *>(oldMaterial);

    OpaqueRaiusImageShader::updateState(state,&newTx->opaqueMaterial,oldTx ? &oldTx->opaqueMaterial : nullptr);

    if (state.isOpacityDirty()) {
        program->setUniformValue(m_opacityId, state.opacity());
    }
}

void RaiusImageShader::initialize()
{
    OpaqueRaiusImageShader::initialize();
    QOpenGLShaderProgram * const program = QSGMaterialShader::program();
    m_opacityId = program->uniformLocation("qt_Opacity");
}

const char *RaiusImageCornerShader::vertexShader() const
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

const char *RaiusImageCornerShader::fragmentShader() const
{
    return "varying highp vec2 qt_TexCoord;                    \n"
           "varying highp vec2 corner;                         \n"
           "uniform lowp vec4 specifyColor;                    \n"
           "uniform highp vec2 blendVec;                       \n"
           "uniform lowp float qt_Opacity;                     \n"
           "uniform lowp vec2 radius;                          \n"
           "uniform sampler2D qt_Texture;                      \n"
           "void main() {                                      \n"
           "    vec4 color = texture2D(qt_Texture, qt_TexCoord);              \n"
           "    lowp vec4 result = vec4(color.xyz * blendVec.x + specifyColor.xyz * blendVec.y, color.w); \n"
           "    gl_FragColor = result * smoothstep(radius.x, radius.y, dot(corner, corner)) * qt_Opacity; \n"
           "}";
}

const char * const *RaiusImageCornerShader::attributeNames() const
{
    static const char *attributes[] = {
        "qt_VertexPosition",
        "qt_VertexTexCoord",
        "vertexCorner",
        nullptr
    };
    return attributes;
}

void RaiusImageCornerShader::updateState(const QSGMaterialShader::RenderState &state, QSGMaterial *newMaterial, QSGMaterial *oldMaterial)
{
    RaiusImageShader::updateState(state, newMaterial, oldMaterial);

    QOpenGLShaderProgram * const program = QSGMaterialShader::program();
    CornerRadiusImageTextureMaterial *newSurface = static_cast<CornerRadiusImageTextureMaterial *>(newMaterial);

    const float radius = newSurface->radius();
    program->setUniformValue(m_radiusId, (radius + 0.5f) * (radius + 0.5f), (radius - 0.5f) * (radius - 0.5f));
}

void RaiusImageCornerShader::initialize()
{
    RaiusImageShader::initialize();
    QOpenGLShaderProgram * const program = QSGMaterialShader::program();
    m_radiusId = program->uniformLocation("radius");
}

QSGMaterialType *QSGRadiusImageOpaqueTextureMaterial::type() const
{
    static QSGMaterialType type;
    return &type;
}

QSGMaterialShader *QSGRadiusImageOpaqueTextureMaterial::createShader() const
{
    return new OpaqueRaiusImageShader;
}

int QSGRadiusImageOpaqueTextureMaterial::compare(const QSGMaterial *other) const
{
    return 1;
}


QSGMaterialType *QSGRadiusImageTextureMaterial::type() const
{
    static QSGMaterialType type;
    return &type;
}

QSGMaterialShader *QSGRadiusImageTextureMaterial::createShader() const
{
    return new RaiusImageShader;
}

int QSGRadiusImageTextureMaterial::compare(const QSGMaterial *other) const
{
    return opaqueMaterial.compare(&static_cast<const QSGRadiusImageTextureMaterial *>(other)->opaqueMaterial);
}

QSGMaterialType *CornerRadiusImageTextureMaterial::type() const
{
    static QSGMaterialType type;
    return &type;
}

QSGMaterialShader *CornerRadiusImageTextureMaterial::createShader() const
{
    return new RaiusImageCornerShader;
}

int CornerRadiusImageTextureMaterial::compare(const QSGMaterial *other) const
{
    const CornerRadiusImageTextureMaterial * const surface = static_cast<const CornerRadiusImageTextureMaterial *>(other);

    const int result = QSGRadiusImageTextureMaterial::compare(other);
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
