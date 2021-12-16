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
#include "dquickinwindowblendblur.h"
#include "private/dblitframebuffernode_p.h"
#include "private/dblurimagenode_p.h"

#include <QQuickWindow>

DQUICK_BEGIN_NAMESPACE

DQuickInWindowBlendBlur::DQuickInWindowBlendBlur(QQuickItem *parent)
    : QQuickItem(parent)
{
    setFlag(QQuickItem::ItemHasContents, true);
}

qreal DQuickInWindowBlendBlur::radius() const
{
    return m_radius;
}

void DQuickInWindowBlendBlur::setRadius(qreal newRadius)
{
    if (qFuzzyCompare(m_radius, newRadius))
        return;

    m_radius = newRadius;
    update();
}

const QColor &DQuickInWindowBlendBlur::blendColor() const
{
    return m_blendColor;
}

void DQuickInWindowBlendBlur::setBlendColor(const QColor &newBlendColor)
{
    if (m_blendColor == newBlendColor)
        return;

    m_blendColor = newBlendColor;
    Q_EMIT blendColorChanged();
    update();
}

static void updateSoftwareNodeTexture(DBlitFramebufferNode *node, void *blurNode) {
    auto bn = reinterpret_cast<DSoftwareBlurImageNode*>(blurNode);
    bn->setTexture(node->texture());
}

QSGNode *DQuickInWindowBlendBlur::updatePaintNode(QSGNode *oldNode, UpdatePaintNodeData *data)
{
    Q_UNUSED(data)

    auto node = static_cast<DBlitFramebufferNode*>(oldNode);
    const QSGRendererInterface::GraphicsApi ga = window()->rendererInterface()->graphicsApi();
    if (Q_UNLIKELY(!node)) {
        if (ga == QSGRendererInterface::Software) {
            node = DBlitFramebufferNode::createSoftwareNode(this, true, true);
            auto blurNode = new DSoftwareBlurImageNode(this);
            blurNode->setDisabledOpaqueRendering(true);
            node->appendChildNode(blurNode);
            node->setRenderCallback(updateSoftwareNodeTexture, blurNode);
        }
    #ifndef QT_NO_OPENGL
        else if (ga == QSGRendererInterface::OpenGL
    #if QT_VERSION >= QT_VERSION_CHECK(5, 14, 0)
                 || ga == QSGRendererInterface::OpenGLRhi
    #endif
                 ) {
            node = DBlitFramebufferNode::createOpenGLNode(this, true, true);
            // TODO: Add the HW blur image node
        }
    #endif
        else {
            qWarning() << "Not supported graphics API:" << ga;
            return nullptr;
        }
    }

    node->resize(size());
    if (ga == QSGRendererInterface::Software) {
        auto blurNode = static_cast<DSoftwareBlurImageNode*>(node->firstChild());
        blurNode->setRadius(m_radius);
        const QRectF rect(0, 0, width(), height());
        blurNode->setSourceRect(rect);
        blurNode->setRect(rect);
        blurNode->setTexture(node->texture());
        blurNode->setBlendColor(m_blendColor);
        blurNode->setFollowMatrixForSource(true);
    } else {
        // TODO
    }

    return node;
}

void DQuickInWindowBlendBlur::itemChange(ItemChange type, const ItemChangeData &data)
{
    if (type == ItemDevicePixelRatioHasChanged) {
        update();
    }

    QQuickItem::itemChange(type, data);
}

DQUICK_END_NAMESPACE
