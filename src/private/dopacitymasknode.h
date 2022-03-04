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

#ifndef DOPACITYMASKNODE_H
#define DOPACITYMASKNODE_H

#include <dtkdeclarative_global.h>

#include <QObject>
#include <QImage>
#include <QSGRenderNode>

QT_BEGIN_NAMESPACE
class QQuickWindow;
class QQuickItem;
class QSGTextureProvider;
class QSGTexture;
QT_END_NAMESPACE

DQUICK_BEGIN_NAMESPACE

class DSoftwareOpacityMaskNode : public QObject, public QSGRenderNode
{
    Q_OBJECT
public:
    explicit DSoftwareOpacityMaskNode(QSGTextureProvider *source, QSGTextureProvider *maskSource);

    void render(const RenderState *state) override;
    StateFlags changedStates() const override { return {}; }
    RenderingFlags flags() const override { return BoundedRectRendering; }
    QRectF rect() const override { return QRect(0, 0, static_cast<int>(m_width), static_cast<int>(m_height)); }
    void setInvert(bool invert);
    void sync(QQuickItem *item);
    void preprocess() override;

public Q_SLOTS:
    void onMarkMaterialToDirty();
    void onSourceTextureProviderDestroyed();
    void onMaskSourceTextureProviderDestroyed();

private:
    QImage updateCachedImage(QSGTextureProvider *provider);
    bool updateTexture(QSGTexture *texture);

private:
    bool m_invert = false;
    qreal m_width = 0;
    qreal m_height = 0;
    QSGTextureProvider *m_sourceProvider = nullptr;
    QSGTextureProvider *m_maskSourceProvider = nullptr;
    QQuickWindow *m_window = nullptr;
    QQuickItem *m_item = nullptr;
    QImage m_source;
    QImage m_maskSource;
};

DQUICK_END_NAMESPACE

#endif // DOPACITYMASKNODE_H
