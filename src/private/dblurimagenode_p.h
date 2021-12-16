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
#ifndef DSOFTWAREBLURIMAGENODE_H
#define DSOFTWAREBLURIMAGENODE_H

#include <dtkdeclarative_global.h>
#include <QSGRenderNode>
#include <QImage>

QT_BEGIN_NAMESPACE
class QQuickItem;
class QSGTexture;
QT_END_NAMESPACE

DQUICK_BEGIN_NAMESPACE

class DSoftwareBlurImageNode : public QSGRenderNode
{
public:
    DSoftwareBlurImageNode(QQuickItem *owner);

    void setTexture(QSGTexture *texture);
    inline QSGTexture *texture() const
    { return m_texture;}
    void setRadius(qreal radius);
    void setSourceRect(const QRectF &source);
    void setRect(const QRectF &target);
    void setDisabledOpaqueRendering(bool disabled);
    void setBlendColor(const QColor &color);
    void setFollowMatrixForSource(bool on);

    void render(const RenderState *state) override;
    void releaseResources() override;
    RenderingFlags flags() const override;
    QRectF rect() const override;

private:
    void updateCachedImage();

    QQuickItem *item;
    QSGTexture *m_texture = nullptr;
    qreal radius = 0;
    QRectF sourceRect;
    QRectF targetRect;
    bool disabledOpaqueRendering = false;
    QColor blendColor;
    bool followMatrixForSource = false;

    QImage cachedSource;
};

DQUICK_END_NAMESPACE

#endif // DSOFTWAREBLURIMAGENODE_H
