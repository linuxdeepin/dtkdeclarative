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
#include "dquickinwindowblur_p.h"
#include "private/dblitframebuffernode_p.h"
#include "private/dblurimagenode_p.h"

#include <private/qquickitem_p.h>
#if (QT_VERSION >= QT_VERSION_CHECK(5, 14, 0))
#include <private/qsgplaintexture_p.h>
#else
#include <private/qsgtexture_p.h>
#endif

#include <QQuickWindow>

DQUICK_BEGIN_NAMESPACE

class Q_DECL_HIDDEN TextureProvider : public QSGTextureProvider {
public:
    TextureProvider()
        : QSGTextureProvider()
        , m_texture(new QSGPlainTexture())
    {
        m_texture->setOwnsTexture(false);
    }

    inline QSGPlainTexture *plainTexture() const {
        return m_texture.data();
    }
    QSGTexture *texture() const override {
        return plainTexture();
    }

private:
    QScopedPointer<QSGPlainTexture> m_texture;
};

DQuickInWindowBlur::DQuickInWindowBlur(QQuickItem *parent)
    : QQuickItem(parent)
{
    setFlag(QQuickItem::ItemHasContents, true);
}

DQuickInWindowBlur::~DQuickInWindowBlur()
{

}

qreal DQuickInWindowBlur::radius() const
{
    return m_radius;
}

void DQuickInWindowBlur::setRadius(qreal newRadius)
{
    if (qFuzzyCompare(m_radius, newRadius))
        return;

    m_radius = newRadius;
    update();
}

bool DQuickInWindowBlur::offscreen() const
{
    return m_offscreen;
}

void DQuickInWindowBlur::setOffscreen(bool newOffscreen)
{
    if (m_offscreen == newOffscreen)
        return;
    m_offscreen = newOffscreen;
    Q_EMIT offscreenChanged();

    update();
}

QSGTextureProvider *DQuickInWindowBlur::textureProvider() const
{
    const QQuickItemPrivate *d = QQuickItemPrivate::get(this);
    if (!d->window || !d->sceneGraphRenderContext() || QThread::currentThread() != d->sceneGraphRenderContext()->thread()) {
        qWarning("DQuickInWindowBlendBlur::textureProvider: can only be queried on the rendering thread of an exposed window");
        return nullptr;
    }

    if (!m_tp) {
        m_tp.reset(new TextureProvider());
    }
    return m_tp.data();
}

static void updateBlurNodeTexture(DBlitFramebufferNode *node, void *blurNode) {
    auto bn = reinterpret_cast<DSGBlurNode*>(blurNode);
    bn->setTexture(node->texture());
}

void onRender(DSGBlurNode *node, void *data) {
    DQuickInWindowBlur *that = reinterpret_cast<DQuickInWindowBlur*>(data);
    if (!that->m_tp)
        return;
    node->writeToTexture(that->m_tp->plainTexture());
    // Don't direct emit the signal, must ensure the signal emit on current render loop after.
    that->m_tp->metaObject()->invokeMethod(that->m_tp.data(), &TextureProvider::textureChanged, Qt::QueuedConnection);
}

QSGNode *DQuickInWindowBlur::updatePaintNode(QSGNode *oldNode, UpdatePaintNodeData *data)
{
    Q_UNUSED(data)

    auto node = static_cast<DBlitFramebufferNode*>(oldNode);
    const QSGRendererInterface::GraphicsApi ga = window()->rendererInterface()->graphicsApi();
    if (Q_UNLIKELY(!node)) {
        if (ga == QSGRendererInterface::Software) {
            node = DBlitFramebufferNode::createSoftwareNode(this, true, true);
            auto blurNode = new DSoftwareBlurImageNode(this);
            blurNode->setDisabledOpaqueRendering(true);
            blurNode->setRenderCallback(onRender, this);
            node->appendChildNode(blurNode);
            node->setRenderCallback(updateBlurNodeTexture, blurNode);
        }
#ifndef QT_NO_OPENGL
        else if (ga == QSGRendererInterface::OpenGL
         #if QT_VERSION >= QT_VERSION_CHECK(5, 14, 0)
                 || ga == QSGRendererInterface::OpenGLRhi
         #endif
                 ) {
            node = DBlitFramebufferNode::createOpenGLNode(this, true, true);
            auto blurNode = new DOpenGLBlurEffectNode(this);
            blurNode->setDisabledOpaqueRendering(true);
            blurNode->setRenderCallback(onRender, this);
            node->appendChildNode(blurNode);
            node->setRenderCallback(updateBlurNodeTexture, blurNode);
        }
#endif
        else {
            qWarning() << "Not supported graphics API:" << ga;
            return nullptr;
        }
    }

    if (!m_tp) {
        m_tp.reset(new TextureProvider());
    }

    node->resize(size());
    DSGBlurNode *blurNode = static_cast<DSGBlurNode*>(node->firstChild());
    Q_ASSERT(blurNode);
    blurNode->setRadius(m_radius);
    const QRectF rect(0, 0, width(), height());
    blurNode->setSourceRect(rect);
    blurNode->setRect(rect);
    blurNode->setTexture(node->texture());
    blurNode->setFollowMatrixForSource(true);
    blurNode->setOffscreen(m_offscreen);

    return node;
}

void DQuickInWindowBlur::itemChange(ItemChange type, const ItemChangeData &data)
{
    if (type == ItemDevicePixelRatioHasChanged) {
        update();
    }

    QQuickItem::itemChange(type, data);
}

DQUICK_END_NAMESPACE
