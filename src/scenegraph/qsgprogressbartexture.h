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

#ifndef QSGPROGRESSBARTEXTURE_H
#define QSGPROGRESSBARTEXTURE_H

#include <QSGMaterial>
#include <QColor>

class OpaqueRectangleTextureMaterial : public QSGMaterial
{
public:
    QColor color() const { return  m_color; }
    qreal value() const { return m_value; }
    QPointF direct() const { return m_direct; }
    void setColor(const QColor &color) { m_color = color; }
    void setValue(qreal value) { m_value = value; }
    void setorientation(Qt::Orientation orientation) {
        if (orientation == Qt::Vertical) {
            m_direct.setX(0.0);
            m_direct.setY(1.0);
        } else {
            m_direct.setX(1.0);
            m_direct.setY(0.0);
        }
    }

    QSGMaterialType *type() const override;
    QSGMaterialShader *createShader() const override;

    int compare(const QSGMaterial *other) const override;

private:
    QColor m_color;
    qreal m_value = 1;
    QPointF m_direct;
};

class RectangleTextureMaterial : public QSGMaterial
{
public:
    RectangleTextureMaterial(OpaqueRectangleTextureMaterial &opaqueMaterial)
        : opaqueMaterial(opaqueMaterial)
    {

    }

    QColor color() const { return  m_color; }
    qreal value() const { return m_value; }
    QPointF direct() const { return m_direct; }
    void setColor(const QColor &color) { m_color = color; }
    void setValue(qreal value) { m_value = value; }
    void setorientation(Qt::Orientation orientation) {
        if (orientation == Qt::Vertical) {
            m_direct.setX(0.0);
            m_direct.setY(1.0);
        } else {
            m_direct.setX(1.0);
            m_direct.setY(0.0);
        }
    }

    QSGMaterialType *type() const override;
    QSGMaterialShader *createShader() const override;

    int compare(const QSGMaterial *other) const override;

    OpaqueRectangleTextureMaterial &opaqueMaterial;

protected:
   QPointF m_direct;

private:
    QColor m_color;
    qreal m_value = 1;
};

class CornerRectangleTextureMaterial : public RectangleTextureMaterial
{
public:
    CornerRectangleTextureMaterial(OpaqueRectangleTextureMaterial &opaqueMaterial)
        : RectangleTextureMaterial(opaqueMaterial)
    {
    }

    float radius() const { return m_radius; }
    void setRadius(float radius) { m_radius = radius; }

    QSGMaterialType *type() const override;
    QSGMaterialShader *createShader() const override;

    int compare(const QSGMaterial *other) const override;

private:
    qreal m_radius = 0;
};

#endif // QSGPROGRESSBARTEXTURE_H
