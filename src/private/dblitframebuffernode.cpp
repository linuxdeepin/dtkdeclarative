/*
 * Copyright (C) 2021 Uniontech Technology Co., Ltd.
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
#include "dblitframebuffernode_p.h"

#include <QSurface>
#ifndef QT_NO_OPENGL
#include <QOpenGLContext>
#include <QOpenGLFunctions>
#include <QOpenGLTexture>
#include <QOpenGLFramebufferObject>
#endif
#include <QQuickItem>
#if QT_VERSION < QT_VERSION_CHECK(5, 14, 0)
#include <private/qsgtexture_p.h>
#else
#include <private/qsgplaintexture_p.h>
#endif
#include <private/qquickitem_p.h>

DQUICK_BEGIN_NAMESPACE

#ifndef QT_NO_OPENGL
class Q_DECL_HIDDEN GLRenderNode : public DBlitFramebufferNode {
public:
    GLRenderNode(QQuickItem *item, const QSize &size)
        : DBlitFramebufferNode(item, size)
    {
        GLRenderNode::reset();
    }

    void render(const RenderState *state) override {
        Q_UNUSED(state)
        if (!QOpenGLFramebufferObject::hasOpenGLFramebufferBlit())
            return;

        const qreal scale = m_item->window()->effectiveDevicePixelRatio();
        const QPoint offset = m_item->mapToScene(QPointF(0, 0)).toPoint();
        const QSize ss = QOpenGLContext::currentContext()->surface()->size() * scale;
        const QRect sourceRect(offset.x(), ss.height() - offset.y(), fbo->width(), -fbo->height());
        QOpenGLFramebufferObject::blitFramebuffer(fbo.data(), QRect(QPoint(0, 0), fbo->size()), nullptr, sourceRect);

        doRenderCallback();
    }
    void reset() override {
        fbo.reset(new QOpenGLFramebufferObject(m_size));
        m_texture->setTextureId(fbo->texture());
        m_texture->setHasAlphaChannel(true);
        m_texture->setTextureSize(fbo->size());
    }

private:
    QScopedPointer<QOpenGLFramebufferObject> fbo;
};

DBlitFramebufferNode *DBlitFramebufferNode::createOpenGLNode(QQuickItem *item, const QSize &size)
{
    return new GLRenderNode(item, size);
}
#endif

class Q_DECL_HIDDEN SoftwareRenderNode : public DBlitFramebufferNode {
public:
    SoftwareRenderNode(QQuickItem *item, const QSize &size)
        : DBlitFramebufferNode(item, size)
        , image(size, QImage::Format_ARGB32_Premultiplied)
        , painter(&image)
    {

    }

    void render(const RenderState *state) override {
        Q_UNUSED(state)
        QSGRendererInterface *rif = m_item->window()->rendererInterface();
        QPainter *p = static_cast<QPainter *>(rif->getResource(m_item->window(),
                                                               QSGRendererInterface::PainterResource));
        Q_ASSERT(p);

        const QPointF offset = m_item->mapToScene(QPointF(0, 0));
        const QRectF sourceRect(offset, m_size);
        const auto device = p->device();
        Q_ASSERT(painter.isActive());

        if (Q_LIKELY(device->devType() == QInternal::Image)) {
            painter.drawImage(image.rect(), *static_cast<QImage*>(device), sourceRect);
        } else if (device->devType() == QInternal::Pixmap) {
            painter.drawPixmap(image.rect(), *static_cast<QPixmap*>(device), sourceRect);
        }

        m_texture->setImage(image);
        doRenderCallback();
    }
    void reset() override {
        painter.end();
        image = QImage(m_size, QImage::Format_ARGB32_Premultiplied);
        painter.begin(&image);
    }

private:
    QImage image;
    QPainter painter;
};

DBlitFramebufferNode *DBlitFramebufferNode::createSoftwareNode(QQuickItem *item, const QSize &size)
{
    return new SoftwareRenderNode(item, size);
}

QRectF DBlitFramebufferNode::rect() const
{
    return QRectF(0, 0, m_item->width(), m_item->height());
}

void DBlitFramebufferNode::resize(const QSize &size)
{
    if (m_size == size)
        return;
    m_size = size;
    reset();
}

void DBlitFramebufferNode::setRenderCallback(RenderCallback callback, void *data)
{
    m_renderCallback = callback;
    m_callbackData = data;
}

DBlitFramebufferNode::DBlitFramebufferNode(QQuickItem *item, const QSize &size)
    : m_item(item)
    , m_size(size)
    , m_texture(new QSGPlainTexture())
{
    m_texture->setOwnsTexture(false);
}

DQUICK_END_NAMESPACE
