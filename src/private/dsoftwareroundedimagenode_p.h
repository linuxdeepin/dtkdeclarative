/*
 * Copyright (C) 2021 Uniontech Technology Co., Ltd.
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
#ifndef DSOFTWAREROUNDEDIMAGENODE_P_H
#define DSOFTWAREROUNDEDIMAGENODE_P_H

#include <dtkdeclarative_global.h>
#include <QSGRenderNode>
#include <QImage>
#include <QPainterPath>

QT_BEGIN_NAMESPACE
class QQuickItem;
class QSGTexture;
QT_END_NAMESPACE

DQUICK_BEGIN_NAMESPACE

class Q_DECL_EXPORT DSoftwareRoundedImageNode : public QSGRenderNode
{
public:
    DSoftwareRoundedImageNode(QQuickItem *owner);

    void setTexture(QSGTexture *texture);
    inline QSGTexture *texture() const
    { return m_texture;}
    void setRadius(qreal radius);
    void setSmooth(bool smooth);
    void setSourceRect(const QRectF &source);
    void setRect(const QRectF &target);

    void render(const RenderState *state) override;
    void releaseResources() override;
    RenderingFlags flags() const override;
    QRectF rect() const override;

private:
    void updateCachedImage();

    qreal radius = 0;
    bool smooth = false;
    QRectF sourceRect;
    QRectF targetRect;
    QPainterPath clipPath;

    QSGTexture *m_texture = nullptr;
    QQuickItem *item = nullptr;
    QImage cachedImage;
};

DQUICK_END_NAMESPACE

#endif // DSOFTWAREROUNDEDIMAGENODE_P_H
