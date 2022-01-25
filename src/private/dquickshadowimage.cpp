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

#include "dquickshadowimage_p.h"
#include "private/dquickshadowimage_p_p.h"

#include <QSGTexture>
#include <QQuickWindow>

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

bool DQuickShadowImage::hollow() const
{
    Q_D(const DQuickShadowImage);

    return d->hollow;
}

void DQuickShadowImage::setHollow(bool hollow)
{
    Q_D(DQuickShadowImage);

    if (hollow == d->hollow)
        return;

    d->hollow = hollow;
    d->needUpdateHollow = true;
    Q_EMIT hollowChanged();
    update();
}

qreal DQuickShadowImage::offsetX() const
{
    Q_D(const DQuickShadowImage);

    return d->offsetX;
}

void DQuickShadowImage::setOffsetX(qreal offset)
{
    Q_D(DQuickShadowImage);

    if (qFuzzyCompare(offset, d->offsetX))
        return;

    d->offsetX = offset;
    Q_EMIT offsetXChanged();
    if (hollow() && !isInner())
        d->needUpdateHollow = true;

    update();
}

qreal DQuickShadowImage::offsetY() const
{
    Q_D(const DQuickShadowImage);

    return d->offsetY;
}

void DQuickShadowImage::setOffsetY(qreal offset)
{
    Q_D(DQuickShadowImage);

    if (qFuzzyCompare(offset, d->offsetY))
        return;

    d->offsetY = offset;
    Q_EMIT offsetYChanged();
    if (hollow() && !isInner())
        d->needUpdateHollow = true;

    update();
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

    d->ensureTextureForShadow();
    if (!(d->shadowTexture && d->shadowTexture->texture))
        return oldNode;

    QImage texture;
    if (d->hollow && !isInner()) {
        texture = d->reprocessHollowShadowTexture();
    } else {
        texture = *d->shadowTexture->texture;
    }

    if (!texture.isNull())
        d->updateCacheShadow(texture);
    if (!d->cacheShadow)
        return oldNode;

    QSGInternalImageNode *node = static_cast<QSGInternalImageNode *>(oldNode);
    if (!node)
#if (QT_VERSION >= QT_VERSION_CHECK(5, 13, 0))
        node = d->sceneGraphContext()->createInternalImageNode(d->sceneGraphRenderContext());
#else
        node = d->sceneGraphContext()->createInternalImageNode();
#endif

    node->setTexture(d->cacheShadow);

    QRectF targetRect;
    QRectF innerTargetRect;
    QRectF innerSourceRect;
    QRectF subSourceRect;
    d->calculateRects(QSize(d->cacheShadow->textureSize().width(), d->cacheShadow->textureSize().height()), QSizeF(width(), height()),
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

void DQuickShadowImagePrivate::calculateRects(const QSize &sourceSize,
                                              const QSizeF &targetSize,
                                              qreal devicePixelRatio,
                                              QRectF *targetRect,
                                              QRectF *innerTargetRect,
                                              QRectF *innerSourceRect,
                                              QRectF *subSourceRect)
{
    Q_UNUSED(devicePixelRatio)

    *innerSourceRect = QRectF(0, 0, 1, 1);
    *targetRect = QRectF(0, 0, targetSize.width(), targetSize.height());
    *innerTargetRect = *targetRect;

    qreal border = isInner ? ((shadowBlur + spread) >= calculateCornerRadius()
                              ? calculateCornerRadius() : (shadowBlur + spread))
                              : (calculateCornerRadius() + shadowBlur);

    qreal borderLeft = border;
    qreal borderRight = border;
    qreal borderTop = border;
    qreal borderBottom = border;

    qreal margin = isInner ? 0.0 : spread;
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

QImage ShadowTextureCache::qt_image_convolute_filter(const QImage &src, const QVector<qreal> &weights, int radius)
{
    int delta = radius ? radius : qFloor(qSqrt(weights.size()) / qreal(2));
    int filterDim = 2 * delta + 1;

    QImage dst = QImage(src.size(), src.format());

    int w = src.width();
    int h = src.height();

    const QRgb *sr = reinterpret_cast<const QRgb *>(src.constBits());
    int srcStride = src.bytesPerLine() / 4;

    QRgb *dr = reinterpret_cast<QRgb *>(dst.bits());
    int dstStride = dst.bytesPerLine() / 4;

    for (int y = 0; y < h; ++y) {
        for (int x = 0; x < w; ++x) {
            int red = 0;
            int green = 0;
            int blue = 0;
            int alpha = 0;

            qreal redF = 0;
            qreal greenF = 0;
            qreal blueF = 0;
            qreal alphaF = 0;

            int sy = y;
            int sx = x;

            for (int cy = 0; cy < filterDim; ++cy) {
                int scy = sy + cy - delta;

                if (scy < 0 || scy >= h)
                    continue;

                const QRgb *sry = sr + scy * srcStride;

                for (int cx = 0; cx < filterDim; ++cx) {
                    int scx = sx + cx - delta;

                    if (scx < 0 || scx >= w)
                        continue;

                    const QRgb col = sry[scx];

                    if (radius) {
                        red += qRed(col);
                        green += qGreen(col);
                        blue += qBlue(col);
                        alpha += qAlpha(col);
                    } else {
                        qreal wt = weights[cy * filterDim + cx];

                        redF += qRed(col) * wt;
                        greenF += qGreen(col) * wt;
                        blueF += qBlue(col) * wt;
                        alphaF += qAlpha(col) * wt;
                    }
                }
            }

            if (radius)
                dr[x] = qRgba(qRound(red * weights[0]), qRound(green * weights[0]), qRound(blue * weights[0]), qRound(alpha * weights[0]));
            else
                dr[x] = qRgba(qRound(redF), qRound(greenF), qRound(blueF), qRound(alphaF));
        }

        dr += dstStride;
    }

    return dst;
}

ShadowTextureCache::TextureData ShadowTextureCache::getShadowTexture(const ShadowTextureCache::ShadowConfig &config, bool cache, bool antialiasing)
{
    Q_UNUSED(antialiasing)

    if ((config.shadowColor.alpha() == 0 || qIsNull(config.shadowBlur))) {
        return TextureData();
    }

    const QString &to_cache_key_key = buildShadowCacheKey(config);
    Texture *texture = nullptr;

    if (m_cache.contains(to_cache_key_key)) {
        texture = m_cache.value(to_cache_key_key);
    }

    if (!texture) {
        QString path = ":/dtk/declarative/shadow/" + to_cache_key_key + ".png";
        if (QFileInfo::exists(path)) {
            QImage *image = new QImage(path);
            texture = new Texture(image);
            texture->cacheKey = to_cache_key_key;
        }
    }

    // TODO(xiaoyaobing): unfortunately, GPU acceleration has not been used yet,
    // and the performance of this approach is worrying
    if (!texture) {
        QImage *shadowImage;
        if (config.inner) {
            qreal pixel, innerPixel;
            if (config.shapeType == ShapeType::Rectangle) {
                pixel = config.cornerRadius * 2.0 + config.shadowBlur * 4.0 + 2 * config.spread;
                innerPixel = pixel - config.shadowBlur * 2.0;
            } else {
                pixel = config.cornerRadius * 2.0 + config.shadowBlur * 2.0;
                innerPixel = pixel - config.shadowBlur * 2.0;
            }

            QImage image(static_cast<int>(pixel), static_cast<int>(pixel), QImage::Format_ARGB32_Premultiplied);
            image.fill(config.shadowColor);

            QPainter shadowPainter(&image);
            shadowPainter.setRenderHint(QPainter::Antialiasing, true);

            shadowPainter.setCompositionMode(QPainter::CompositionMode_Clear);
            QRectF rectangle(config.shadowBlur + config.spread, config.shadowBlur + config.spread,
                             innerPixel - 2 * config.spread, innerPixel - 2 * config.spread);
            QPainterPath path;
            path.addRoundedRect(rectangle, config.cornerRadius, config.cornerRadius);
            QPen pen(Qt::transparent);
            shadowPainter.setPen(pen);
            shadowPainter.fillPath(path, Qt::red);
            shadowPainter.drawPath(path);
            shadowPainter.end();

            qt_image_boxblur(image, qMax(1, qRound(config.shadowBlur / 3 - 3.0)), true);
            shadowImage = new QImage(static_cast<int>(innerPixel), static_cast<int>(innerPixel), QImage::Format_ARGB32_Premultiplied);
            *shadowImage = image.copy(static_cast<int>(config.shadowBlur), static_cast<int>(config.shadowBlur),
                                     static_cast<int>(innerPixel), static_cast<int>(innerPixel));

            QPainter scissor(shadowImage);
            scissor.setRenderHint(QPainter::Antialiasing, true);
            scissor.setCompositionMode(QPainter::CompositionMode_Clear);
            QPainterPath path1;
            scissor.setPen(Qt::NoPen);

            path1.moveTo(0, 0);
            path1.arcTo(0, 0,  config.cornerRadius * 2,  config.cornerRadius * 2, 90, 90);
            path1.lineTo(0, 0);

            path1.moveTo(shadowImage->width(), 0);
            QRectF f(shadowImage->width() -  config.cornerRadius * 2, 0,
                     config.cornerRadius * 2,  config.cornerRadius * 2);
            path1.arcTo(f, 0, 90);
            path1.lineTo(shadowImage->width(), 0);

            path1.moveTo(0, shadowImage->height());
            QRectF f1(0, shadowImage->height() -  config.cornerRadius * 2,
                      config.cornerRadius * 2, config.cornerRadius * 2);
            path1.arcTo(f1, 180, 90);
            path1.lineTo(0, shadowImage->height());

            path1.moveTo(shadowImage->width(), shadowImage->height());
            QRectF f2(shadowImage->width() -  config.cornerRadius * 2,
                      shadowImage->height() -  config.cornerRadius * 2,
                      config.cornerRadius * 2,  config.cornerRadius * 2);
            path1.arcTo(f2, 270, 90);
            path1.lineTo(shadowImage->width(), shadowImage->height());
            scissor.fillPath(path1, Qt::red);
            scissor.end();
        } else {
            qreal pixel, innerPixel;
            if (config.shapeType == ShapeType::Rectangle) {
                pixel = config.cornerRadius * 2.0 + config.shadowBlur * 4.0;
                innerPixel = pixel - config.shadowBlur * 2.0;
            } else {
                pixel = config.cornerRadius * 2.0 + config.shadowBlur * 2.0;
                innerPixel = pixel - config.shadowBlur * 2.0;
            }

            shadowImage = new QImage(static_cast<int>(pixel), static_cast<int>(pixel), QImage::Format_ARGB32_Premultiplied);
            shadowImage->fill(Qt::transparent);

            QPainter shadowPainter(shadowImage);
            shadowPainter.setRenderHint(QPainter::Antialiasing, true);
            shadowPainter.setPen(Qt::NoPen);

            QRectF rectangle(config.shadowBlur, config.shadowBlur, innerPixel, innerPixel);
            QPainterPath path;
            path.addRoundedRect(rectangle, config.cornerRadius, config.cornerRadius);
            shadowPainter.fillPath(path, config.shadowColor);
            shadowPainter.end();
            if (config.shadowBlur > 0) {
                qt_image_boxblur(*shadowImage, qMax(1, qRound(config.shadowBlur / 3 - 3.0)), true);
            }
        }

        texture = new Texture(shadowImage);
        texture->cacheKey = to_cache_key_key;
        if (cache && !m_cache.contains(to_cache_key_key))
            m_cache[to_cache_key_key] = texture;
    }

    TextureData data(texture);
    return data;
}

DQUICK_END_NAMESPACE
