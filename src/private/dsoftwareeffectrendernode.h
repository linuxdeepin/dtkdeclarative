/*
 * Copyright (C) 2022 UnionTech Technology Co., Ltd.
 *
 * Author:     Chen Bin <chenbin@uniontech.com>
 *
 * Maintainer: Chen Bin <chenbin@uniontech.com>
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

#ifndef DSOFTWARERENDERNODE_H
#define DSOFTWARERENDERNODE_H

#include <dtkdeclarative_global.h>

#include <QObject>
#include <QImage>
#include <QSGRenderNode>
#include <QPointer>
#include <QPainter>
#include <QSGTextureProvider>

QT_BEGIN_NAMESPACE
class QQuickWindow;
class QQuickItem;
//class QSGTextureProvider;
class QSGTexture;
QT_END_NAMESPACE

DQUICK_BEGIN_NAMESPACE

class DSoftwareEffectRenderNode : public QObject, public QSGRenderNode
{
    Q_OBJECT
public:
    explicit DSoftwareEffectRenderNode(QSGTextureProvider *sourceProvider);

    RenderingFlags flags() const override { return BoundedRectRendering; }
    void preprocess() override;
    StateFlags changedStates() const override { return {}; }
    virtual QImage process(const QImage &sourceImage, QPainter::RenderHints hints) = 0;
    virtual void sync(QQuickItem *item);
    void render(const RenderState *state) override final;

    void setCached(bool cached);
    bool cached() const;

    void markChanged();

protected Q_SLOTS:
    void onRenderTextureChanged();

protected:
    QImage updateCachedImage(QSGTextureProvider *provider);
    bool updateTexture(QSGTexture *texture);

private:
    void renderImpl(QPainter *p);

private:
    QPointer<QSGTextureProvider> m_sourceProvider;
    QImage m_sourceImage;
    QImage m_renderDevice;
    QQuickItem *m_item = nullptr;
    QQuickWindow *m_window = nullptr;
    bool m_cached = false;
    bool m_changed = true;
    bool m_antialiasing = false;
    bool m_smooth = false;
};

DQUICK_END_NAMESPACE

#endif // DSOFTWARERENDERNODE_H
