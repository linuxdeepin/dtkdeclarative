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

class ProxyAtlasTexture;
class DBlitFramebufferNode : public QSGRenderNode {
public:
    inline QSizeF size() const {
        return m_size;
    }
    QSGTexture *texture() const;

#ifndef QT_NO_OPENGL
    static DBlitFramebufferNode *createOpenGLNode(QQuickItem *item,
                                                  bool shareBuffer = false,
                                                  bool useAtlasTexture = false);
#endif
    static DBlitFramebufferNode *createSoftwareNode(QQuickItem *item,
                                                    bool shareBuffer = false,
                                                    bool useAtlasTexture = false);

    QRectF rect() const override;

    void resize(const QSizeF &size);
    void setMargins(const QMarginsF &margin);

    typedef void(*RenderCallback)(DBlitFramebufferNode *node, void *data);
    void setRenderCallback(RenderCallback callback, void *data);
    inline void doRenderCallback() {
        if (!m_renderCallback)
            return;
        m_renderCallback(this, m_callbackData);
    }

protected:
    DBlitFramebufferNode(QQuickItem *item);

    QQuickItem *m_item;
    QSizeF m_size;
    QMarginsF m_margins;
    QRectF m_rect;
    QScopedPointer<ProxyAtlasTexture> m_texture;
    RenderCallback m_renderCallback = nullptr;
    void *m_callbackData = nullptr;
    bool shareBuffer = false;
    bool useAtlasTexture = false;
};

DQUICK_END_NAMESPACE

#endif // DBLITFRAMEBUFFERNODE_H
