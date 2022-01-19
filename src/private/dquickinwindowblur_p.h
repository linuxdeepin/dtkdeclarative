/*
 * Copyright (C) 2021 ~ 2022 UnionTech Technology Co., Ltd.
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
#ifndef DQUICKINWINDOWBLUR_P_H
#define DQUICKINWINDOWBLUR_P_H

#include <dtkdeclarative_global.h>

#include <QQuickItem>

QT_BEGIN_NAMESPACE
class QSGPlainTexture;
#ifndef QT_NO_OPENGL
class QOpenGLFramebufferObject;
#endif
QT_END_NAMESPACE

DQUICK_BEGIN_NAMESPACE

class DQuickInWindowBlendBlurPrivate;
class TextureProvider;
class DSGBlurNode;
class DQuickInWindowBlur : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(qreal radius READ radius WRITE setRadius NOTIFY radiusChanged)
    Q_PROPERTY(bool offscreen READ offscreen WRITE setOffscreen NOTIFY offscreenChanged)

public:
    explicit DQuickInWindowBlur(QQuickItem *parent = nullptr);
    ~DQuickInWindowBlur() override;

    qreal radius() const;
    void setRadius(qreal newRadius);

    bool offscreen() const;
    void setOffscreen(bool newOffscreen);

    bool isTextureProvider() const override { return true; }
    QSGTextureProvider *textureProvider() const override;

Q_SIGNALS:
    void radiusChanged();
    void offscreenChanged();

protected:
    QSGNode *updatePaintNode(QSGNode *, UpdatePaintNodeData *) override;
    void itemChange(ItemChange, const ItemChangeData &) override;

private:
    qreal m_radius = 20;
    bool m_offscreen = false;
    mutable QScopedPointer<TextureProvider> m_tp;
    friend void onRender(DSGBlurNode *, void *);
};

DQUICK_END_NAMESPACE

#endif // DQUICKINWINDOWBLUR_P_H
