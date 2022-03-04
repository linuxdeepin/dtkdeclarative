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

#include "dopacitymasknode.h"

#include <private/qsgadaptationlayer_p.h>
#if QT_VERSION < QT_VERSION_CHECK(5, 14, 0)
#include <private/qsgtexture_p.h>
#else
#include <private/qsgplaintexture_p.h>
#endif

#include <QSGTextureProvider>
#include <QQuickWindow>
#include <QQuickItem>

DQUICK_BEGIN_NAMESPACE

DSoftwareOpacityMaskNode::DSoftwareOpacityMaskNode(QSGTextureProvider *source, QSGTextureProvider *maskSource)
    : m_sourceProvider(source)
    , m_maskSourceProvider(maskSource)
{
    setFlag(QSGNode::UsePreprocess, true);

    connect(source, &QSGTextureProvider::textureChanged
            , this, &DSoftwareOpacityMaskNode::onMarkMaterialToDirty, Qt::DirectConnection);
    connect(source, &QSGTextureProvider::destroyed, this
            , &DSoftwareOpacityMaskNode::onSourceTextureProviderDestroyed, Qt::DirectConnection);
    connect(maskSource, &QSGTextureProvider::textureChanged
            , this, &DSoftwareOpacityMaskNode::onMarkMaterialToDirty, Qt::DirectConnection);
    connect(maskSource, &QSGTextureProvider::destroyed, this
            , &DSoftwareOpacityMaskNode::onMaskSourceTextureProviderDestroyed, Qt::DirectConnection);
}

void DSoftwareOpacityMaskNode::render(const QSGRenderNode::RenderState *state)
{
    Q_ASSERT(m_window);

    if (Q_UNLIKELY(!m_sourceProvider || !m_maskSourceProvider))
        return;

    QSGRendererInterface *rif = m_window->rendererInterface();
    QPainter *p = static_cast<QPainter *>(rif->getResource(m_window, QSGRendererInterface::PainterResource));
    Q_ASSERT(p);

    m_source = updateCachedImage(m_sourceProvider);
    m_maskSource = updateCachedImage(m_maskSourceProvider);
    if (Q_UNLIKELY(m_source.isNull() || m_maskSource.isNull() || m_source.rect() != m_maskSource.rect()))
        return;

    const QRegion *clipRegion = state->clipRegion();
    if (Q_UNLIKELY(clipRegion && !clipRegion->isEmpty()))
        p->setClipRegion(*clipRegion, Qt::ReplaceClip);

    p->setTransform(matrix()->toTransform());
    p->setOpacity(inheritedOpacity());

    QPainter painter(&m_source);
    painter.setRenderHint(QPainter::Antialiasing, m_item->antialiasing());
    painter.setPen(Qt::NoPen);
    if (Q_UNLIKELY(!m_invert)) {
        painter.setCompositionMode(QPainter::CompositionMode_DestinationIn);
    } else {
        painter.setCompositionMode(QPainter::CompositionMode_DestinationOut);
    }

    painter.drawImage(rect(), m_maskSource, m_maskSource.rect());
    painter.end();
    p->drawImage(rect(), m_source, m_source.rect());
}

void DSoftwareOpacityMaskNode::setInvert(bool invert)
{
    if (invert == m_invert)
        return;

    m_invert = invert;
    markDirty(DirtyMaterial);
}

void DSoftwareOpacityMaskNode::sync(QQuickItem *item)
{
    m_window = item->window();
    m_width = item->width();
    m_height = item->height();
    m_item = item;
}

void DSoftwareOpacityMaskNode::preprocess()
{
    bool sourceDirty = false;
    bool maskSourceDirty = false;
    if (m_sourceProvider) {
        sourceDirty = updateTexture(m_sourceProvider->texture());
    }

    if (m_maskSourceProvider) {
        maskSourceDirty = updateTexture(m_maskSourceProvider->texture());
    }

    if (sourceDirty || maskSourceDirty)
        markDirty(DirtyMaterial);
}

void DSoftwareOpacityMaskNode::onMarkMaterialToDirty()
{
    markDirty(QSGNode::DirtyMaterial);
}

void DSoftwareOpacityMaskNode::onSourceTextureProviderDestroyed()
{
    m_sourceProvider = nullptr;
}

void DSoftwareOpacityMaskNode::onMaskSourceTextureProviderDestroyed()
{
    m_maskSourceProvider = nullptr;
}

QImage DSoftwareOpacityMaskNode::updateCachedImage(QSGTextureProvider *provider)
{
    if (provider) {
        if (auto texture = qobject_cast<QSGPlainTexture*>(provider->texture()))
            return texture->image();

        if (QSGLayer *layer = qobject_cast<QSGLayer*>(provider->texture()))
            return layer->toImage();
    }

    static QImage nullImage;
    return nullImage;
}

bool DSoftwareOpacityMaskNode::updateTexture(QSGTexture *texture)
{
    bool dirty = false;
    QSGLayer *t = qobject_cast<QSGLayer *>(texture);
    if (t) {
        t->setMirrorVertical(false);
        dirty = t->updateTexture();
    }
    return dirty;
}

DQUICK_END_NAMESPACE
