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

#include "dquickshadowimage.h"
#include "private/dquickshadowimage_p.h"

#include <QSGTexture>
#include <QQuickWindow>
#include <QFuture>
#include <QtConcurrent>
#include <QFutureWatcher>

DQUICK_BEGIN_NAMESPACE

DQuickShadowImage::DQuickShadowImage(QQuickItem *parent)
    : QQuickItem(*(new DQuickShadowImagePrivate), parent)
{
    setFlag(ItemHasContents);
}

bool DQuickShadowImage::cache()
{
    Q_D(const DQuickShadowImage);

    return d->cache;
}

void DQuickShadowImage::setCache(bool cache)
{
    Q_D(DQuickShadowImage);

    if (cache == d->cache)
        return;

    d->cache = cache;
    Q_EMIT cacheChanged();
}

qreal DQuickShadowImage::shadowBlur() const
{
    Q_D(const DQuickShadowImage);

    return d->shadowBlur;
}

void DQuickShadowImage::setShadowBlur(qreal blur)
{
    Q_D(DQuickShadowImage);

    if (qFuzzyCompare(d->shadowBlur, blur))
        return;

    d->shadowBlur = blur;
    d->needUpdateShadow = true;
    update();
    Q_EMIT shadowBlurChanged();
}

QColor DQuickShadowImage::shadowColor() const
{
    Q_D(const DQuickShadowImage);

    return d->shadowColor;
}

void DQuickShadowImage::setShadowColor(const QColor &color)
{
    Q_D(DQuickShadowImage);

    if (color == d->shadowColor)
        return;

    d->shadowColor = color;
    d->needUpdateShadow = true;
    update();
    Q_EMIT shadowColorChanged();
}

bool DQuickShadowImage::isInner()
{
    Q_D(const DQuickShadowImage);

    return d->isInner;
}

void DQuickShadowImage::setIsInner(bool inner)
{
    Q_D(DQuickShadowImage);

    if (inner == d->isInner)
        return;

    d->isInner = inner;
    d->needUpdateShadow = true;
    update();
    Q_EMIT isInnerChanged();
}

qreal DQuickShadowImage::cornerRadius()
{
    Q_D(const DQuickShadowImage);

    return d->cornerRadius;
}

void DQuickShadowImage::setCornerRadius(qreal radius)
{
    Q_D(DQuickShadowImage);

    if (qFuzzyCompare(radius, d->cornerRadius))
        return;

    d->cornerRadius = radius;
    d->needUpdateShadow = true;
    update();
    Q_EMIT cornerRadiusChanded();
}

qreal DQuickShadowImage::spread() const
{
    Q_D(const DQuickShadowImage);

    return d->spread;
}

void DQuickShadowImage::setSpread(qreal spread)
{
    Q_D(DQuickShadowImage);

    if (qFuzzyCompare(spread, d->spread))
        return;

    d->spread = spread;
    update();
    Q_EMIT spreadChanged();
}

DQuickShadowImage::DQuickShadowImage(DQuickShadowImagePrivate &dd, QQuickItem *parent)
    : QQuickItem (dd, parent)
{
}

QSGNode *DQuickShadowImage::updatePaintNode(QSGNode *oldNode, QQuickItem::UpdatePaintNodeData *)
{
    Q_D(DQuickShadowImage);

    if (width() <= 0 || height() <= 0 || d->shadowColor.alpha() == 0) {
        delete oldNode;
        return nullptr;
    }

    if (d->needUpdateShadow) {
        if (d->hasCache()) {
            d->ensureTextureForShadow();
        } else {
            QFutureWatcher<QSGTexture *> *watcher =  new QFutureWatcher<QSGTexture *>();
            connect(watcher, &QFutureWatcher<QSGTexture *>::finished, this, [watcher, this]{
                this->update();
                watcher->deleteLater();
            });

            d->shadowTexture = nullptr;
            QFuture<QSGTexture *> future = QtConcurrent::run(d, &DQuickShadowImagePrivate::ensureTextureForShadow);
            watcher->setFuture(future);
        }
    }

    QSGTexture *texture = d->shadowTexture ? d->shadowTexture->texture : nullptr;

    if (!texture) {
        delete oldNode;
        return nullptr;
    }

    QSGInternalImageNode *node = static_cast<QSGInternalImageNode *>(oldNode);
    if (!node)
#if (QT_VERSION >= QT_VERSION_CHECK(5, 13, 0))
        node = d->sceneGraphContext()->createInternalImageNode(d->sceneGraphRenderContext());
#else
        node = d->sceneGraphContext()->createInternalImageNode();
#endif

    node->setTexture(texture);

    QRectF targetRect;
    QRectF innerTargetRect;
    QRectF innerSourceRect;
    QRectF subSourceRect;
    d->calculateRects(QSize(texture->textureSize().width(), texture->textureSize().height()), QSizeF(width(), height()),
                      window()->effectiveDevicePixelRatio(),
                      &targetRect, &innerTargetRect,
                      &innerSourceRect, &subSourceRect);

    node->setTargetRect(targetRect);
    node->setInnerSourceRect(innerSourceRect);
    node->setInnerTargetRect(innerTargetRect);
    node->setSubSourceRect(subSourceRect);

    node->setMipmapFiltering(QSGTexture::None);
    node->setFiltering(d->smooth ? QSGTexture::Linear : QSGTexture::Nearest);
    if (innerSourceRect == QRectF(0, 0, 1, 1) && (subSourceRect.width() > 1 || subSourceRect.height() > 1)) {
        node->setHorizontalWrapMode(QSGTexture::Repeat);
        node->setVerticalWrapMode(QSGTexture::Repeat);
    } else {
        node->setHorizontalWrapMode(QSGTexture::ClampToEdge);
        node->setVerticalWrapMode(QSGTexture::ClampToEdge);
    }
    node->setAntialiasing(d->antialiasing);
    node->update();

    return node;
}

bool DQuickShadowImage::shapeIsCircular()
{
    Q_D(const DQuickShadowImage);

    if (d->isInner)
        return d->cornerRadius * 2 >= std::min({width(), height()});

    return d->cornerRadius * 2 >= std::min({width() - d->shadowBlur * 2.0, height() - d->shadowBlur * 2.0});
}

qreal DQuickShadowImage::calculateCornerRadius()
{
    Q_D(const DQuickShadowImage);

    if (d->isInner)
        return std::min({d->cornerRadius, std::min({width() / 2.0, height() / 2.0})});

    return std::min({d->cornerRadius, std::min({(width() - 2.0 * d->shadowBlur) / 2.0,
                                                (height() - 2.0 * d->shadowBlur) / 2.0})});
}

void DQuickShadowImagePrivate::calculateRects(const QSize &sourceSize,
                                              const QSizeF &targetSize,
                                              qreal devicePixelRatio,
                                              QRectF *targetRect,
                                              QRectF *innerTargetRect,
                                              QRectF *innerSourceRect,
                                              QRectF *subSourceRect)
{
    Q_Q(DQuickShadowImage);

    *innerSourceRect = QRectF(0, 0, 1, 1);
    *targetRect = QRectF(0, 0, targetSize.width(), targetSize.height());
    *innerTargetRect = *targetRect;

    qreal border = q->calculateCornerRadius() + shadowBlur;
    if (isInner && (q->calculateCornerRadius() + shadowBlur) >= std::min({q->width() / 2.0, q->height() / 2.0}))
        border = q->calculateCornerRadius();

    qreal borderLeft = border;
    qreal borderRight = border;
    qreal borderTop = border;
    qreal borderBottom = border;
    qreal margin =  std::min({qAbs(spread), shadowBlur});

    if (margin * spread < 0)
        margin = -margin;

    if (isInner)
        margin = -margin;

    if (borderLeft + borderRight > sourceSize.width() && borderLeft < sourceSize.width())
        borderRight = sourceSize.width() - borderLeft;
    if (borderTop + borderBottom > sourceSize.height() && borderTop < sourceSize.height())
        borderBottom = sourceSize.height() - borderTop;
    *innerSourceRect = QRectF(QPointF((borderLeft) / qreal(sourceSize.width()),
                                      (borderTop) / qreal(sourceSize.height())),
                              QPointF((sourceSize.width() - borderRight - 1) / qreal(sourceSize.width()),
                                      (sourceSize.height() - borderBottom - 1) / qreal(sourceSize.height())));
    *innerTargetRect = QRectF(border - margin,
                              border - margin,
                              qMax<qreal>(0, targetSize.width() - (border * 2.0) + 2 * margin),
                              qMax<qreal>(0, targetSize.height() - (border * 2.0) + 2 * margin));

    qreal hTiles = 1 - 2 * margin / sourceSize.width();
    qreal vTiles = 1 - 2 * margin / sourceSize.height();
    if (innerSourceRect->width() <= 0)
        hTiles = 0;

    if (innerSourceRect->height() <= 0)
        vTiles = 0;

    *subSourceRect = QRectF((1 - hTiles) / 2, (1 - vTiles) / 2, hTiles, vTiles);
}

DQUICK_END_NAMESPACE
