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
#ifndef DBLITFRAMEBUFFERNODE_H
#define DBLITFRAMEBUFFERNODE_H

#include <dtkdeclarative_global.h>

#include <QSGTextureProvider>
#include <QSGRenderNode>

QT_BEGIN_NAMESPACE
class QSGPlainTexture;
class QQuickItem;
QT_END_NAMESPACE

DQUICK_BEGIN_NAMESPACE

class DBlitFramebufferNode : public QSGRenderNode {
public:
    inline QSize size() const {
        return m_size;
    }
    inline QSGPlainTexture *texture() const {
        return m_texture.data();
    }

#ifndef QT_NO_OPENGL
    static DBlitFramebufferNode *createOpenGLNode(QQuickItem *item, const QSize &size);
#endif
    static DBlitFramebufferNode *createSoftwareNode(QQuickItem *item, const QSize &size);

    QRectF rect() const override;

    void resize(const QSize &size);

    typedef void(*RenderCallback)(void *data);
    void setRenderCallback(RenderCallback callback, void *data);
    inline void doRenderCallback() {
        if (!m_renderCallback)
            return;
        m_renderCallback(m_callbackData);
    }

protected:
    DBlitFramebufferNode(QQuickItem *item, const QSize &textureSize);
    virtual void reset() = 0;

    QQuickItem *m_item;
    QSize m_size;
    QScopedPointer<QSGPlainTexture> m_texture;
    RenderCallback m_renderCallback = nullptr;
    void *m_callbackData = nullptr;
};

DQUICK_END_NAMESPACE

#endif // DBLITFRAMEBUFFERNODE_H
