/*
 * Copyright (C) 2021 UnionTech Technology Co., Ltd.
 *
 * Author:     zccrs <zccrs@live.com>
 *
 * Maintainer: zccrs <zhangjide@uniontech.com>
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
#include "dquickblitframebuffer.h"
#include "private/dblitframebuffernode_p.h"
#include <DObjectPrivate>

#include <QDebug>
#include <private/qquickwindow_p.h>
#if QT_VERSION < QT_VERSION_CHECK(5, 14, 0)
#include <private/qsgtexture_p.h>
#else
#include <private/qsgplaintexture_p.h>
#endif

DQUICK_BEGIN_NAMESPACE

class Q_DECL_HIDDEN TextureProvider : public QSGTextureProvider {
public:
    TextureProvider()
        : QSGTextureProvider()
    {

    }

    QSGTexture *texture() const override {
        return m_texture;
    }
    inline void setTexture(QSGTexture *tex) {
        m_texture = tex;
    }

private:
    QSGTexture *m_texture = nullptr;
};


class Q_DECL_HIDDEN DQuickBlitFramebufferPrivate : public DCORE_NAMESPACE::DObjectPrivate
{
public:
    DQuickBlitFramebufferPrivate(DQuickBlitFramebuffer *qq)
        : DObjectPrivate(qq)
    {

    }

    mutable QScopedPointer<TextureProvider> tp;
};

DQuickBlitFramebuffer::DQuickBlitFramebuffer(QQuickItem *parent)
    : QQuickItem(parent)
    , DObject(*new DQuickBlitFramebufferPrivate(this))
{
    setFlag(ItemHasContents);
}

bool DQuickBlitFramebuffer::isTextureProvider() const
{
    return true;
}

QSGTextureProvider *DQuickBlitFramebuffer::textureProvider() const
{
    D_DC(DQuickBlitFramebuffer);

    QQuickWindow *w = window();
    if (!w || !w->isSceneGraphInitialized()
            || QThread::currentThread() != QQuickWindowPrivate::get(w)->context->thread()) {
        qWarning("DQuickBlitFramebuffer::textureProvider: can only be queried on the rendering thread of an exposed window");
        return nullptr;
    }

    if (!d->tp) {
        d->tp.reset(new TextureProvider());
    }

    return d->tp.data();
}

static void onRender(void *data) {
    TextureProvider *tp = reinterpret_cast<TextureProvider*>(data);
    Q_EMIT tp->textureChanged();
}

QSGNode *DQuickBlitFramebuffer::updatePaintNode(QSGNode *oldNode, QQuickItem::UpdatePaintNodeData *oldData)
{
    Q_UNUSED(oldData)

    auto node = static_cast<DBlitFramebufferNode*>(oldNode);
    const QSize textureSize = (size() * window()->effectiveDevicePixelRatio()).toSize();
    if (Q_LIKELY(node)) {
        node->resize(textureSize);
        return node;
    }

    D_DC(DQuickBlitFramebuffer);
    auto ri = window()->rendererInterface();
    if (ri->graphicsApi() == QSGRendererInterface::Software) {
        node = DBlitFramebufferNode::createSoftwareNode(this, textureSize);
    }
#ifndef QT_NO_OPENGL
    else if (ri->graphicsApi() == QSGRendererInterface::OpenGL
#if QT_VERSION >= QT_VERSION_CHECK(5, 14, 0)
             || ri->graphicsApi() == QSGRendererInterface::OpenGLRhi
#endif
             ) {
        node = DBlitFramebufferNode::createOpenGLNode(this, textureSize);
    }
#endif
    else {
        qWarning() << "Not supported graphics API:" << ri->graphicsApi();
        return nullptr;
    }

    if (!d->tp) {
        d->tp.reset(new TextureProvider());
    }
    node->setRenderCallback(onRender, d->tp.data());
    d->tp->setTexture(node->texture());

    return node;
}

void DQuickBlitFramebuffer::itemChange(ItemChange type, const ItemChangeData &data)
{
    if (type == ItemDevicePixelRatioHasChanged) {
        update();
    }

    QQuickItem::itemChange(type, data);
}

DQUICK_END_NAMESPACE
