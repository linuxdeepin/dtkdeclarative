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

#ifndef DQUICKSHADOWIMAGE_P_H
#define DQUICKSHADOWIMAGE_P_H

#include "dquickshadowimage.h"

#include <private/qquickitem_p.h>

#include <private/qquickitem_p.h>

#include <QString>

DQUICK_BEGIN_NAMESPACE

class ShadowTextureCache {
public:
    enum ShapeType {
        Rectangle,
        Circular
    };

    struct ShadowConfig {
        qreal cornerRadius;
        qreal shadowBlur;
        QColor shadowColor;
        bool inner;
        ShapeType shapeType = ShapeType::Rectangle;
    };

    class Texture : public QSharedData {
    public:
        Texture(QSGTexture *t)
            : texture(t)
        {}

        ~Texture() {
            ShadowTextureCache::instance()->m_cache.remove(cacheKey);
            delete texture;
        }

        QString cacheKey;
        QSGTexture *texture;
    };

    typedef QExplicitlySharedDataPointer<Texture> TextureData;

    static ShadowTextureCache *instance()
    {
        static ShadowTextureCache *object = new ShadowTextureCache();
        return object;
    }

    QString buildShadowCacheKey(const ShadowConfig &config)
    {
        return QString("%1.%2.%3.%4.%5").arg(config.cornerRadius)
                .arg(config.shadowBlur).arg(config.shadowColor.name()).arg(config.inner).arg(config.shapeType);
    }

    QImage qt_image_convolute_filter(const QImage& src, const QVector<qreal>& weights, int radius = 0)
    {
        int delta = radius ? radius : qFloor(qSqrt(weights.size()) / qreal(2));
        int filterDim = 2 * delta + 1;

        QImage dst = QImage(src.size(), src.format());

        int w = src.width();
        int h = src.height();

        const QRgb *sr = (const QRgb *)(src.constBits());
        int srcStride = src.bytesPerLine() / 4;

        QRgb *dr = (QRgb*)dst.bits();
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

    void qt_image_boxblur(QImage& image, int radius, bool quality)
    {
        int passes = quality? 3: 1;
        int filterSize = 2 * radius + 1;
        for (int i = 0; i < passes; ++i)
            image = qt_image_convolute_filter(image, QVector<qreal>() << 1.0 / (filterSize * filterSize), radius);
    }

    bool keyIsCache(const ShadowConfig &config)
    {
        return m_cache.contains(buildShadowCacheKey(config));
    }

    TextureData getShadowTexture(QSGRenderContext *context, const ShadowConfig &config, bool cache = false, bool antialiasing = true)
    {
        if ((config.shadowColor.alpha() == 0 || qIsNull(config.shadowBlur))) {
            return TextureData();
        }

        const QString &to_cache_key_key = buildShadowCacheKey(config);
        Texture *texture = nullptr;

        if (m_cache.contains(to_cache_key_key)) {
            texture = m_cache.value(to_cache_key_key);
        }

        // TODO(xiaoyaobing): unfortunately, GPU acceleration has not been used yet,
        // and the performance of this approach is worrying
        if (!texture) {
            QImage shadowImage;
            if (config.inner) {
                qreal pixel, innerPixel;
                if (config.shapeType == ShapeType::Rectangle) {
                    pixel = config.cornerRadius * 2.0 + config.shadowBlur * 6.0;
                    innerPixel = pixel - config.shadowBlur * 2.0;
                } else {
                    pixel = config.cornerRadius * 2.0 + config.shadowBlur * 2.0;
                    innerPixel = pixel - config.shadowBlur * 2.0;
                }

                QImage image(pixel, pixel, QImage::Format_ARGB32_Premultiplied);
                image.fill(config.shadowColor);

                QPainter shadowPainter(&image);
                shadowPainter.setRenderHint(QPainter::Antialiasing, true);

                shadowPainter.setCompositionMode(QPainter::CompositionMode_Clear);
                QRectF rectangle(config.shadowBlur, config.shadowBlur, innerPixel, innerPixel);
                QPainterPath path;
                path.addRoundedRect(rectangle, config.cornerRadius, config.cornerRadius);
                QPen pen(Qt::transparent);
                shadowPainter.setPen(pen);
                shadowPainter.fillPath(path, Qt::red);
                shadowPainter.drawPath(path);
                shadowPainter.end();

                qt_image_boxblur(image, qMax(1, qRound(config.shadowBlur / 2)), true);
                shadowImage = QImage(innerPixel, innerPixel, QImage::Format_ARGB32_Premultiplied);
                shadowImage = image.copy(config.shadowBlur, config.shadowBlur,
                                         innerPixel, innerPixel);

                QPainter scissor(&shadowImage);
                scissor.setRenderHint(QPainter::Antialiasing, true);
                scissor.setCompositionMode(QPainter::CompositionMode_Clear);
                QPainterPath path1;
                scissor.setPen(QPen(Qt::transparent));

                path1.moveTo(0, 0);
                path1.arcTo(0, 0,  config.cornerRadius * 2,  config.cornerRadius * 2, 90, 90);
                path1.lineTo(0, 0);

                path1.moveTo(shadowImage.width(), 0);
                QRectF f(shadowImage.width() -  config.cornerRadius * 2, 0,
                         config.cornerRadius * 2,  config.cornerRadius * 2);
                path1.arcTo(f, 0, 90);
                path1.lineTo(shadowImage.width(), 0);

                path1.moveTo(0, shadowImage.height());
                QRectF f1(0, shadowImage.height() -  config.cornerRadius * 2,
                          config.cornerRadius * 2, config.cornerRadius * 2);
                path1.arcTo(f1, 180, 90);
                path1.lineTo(0, shadowImage.height());

                path1.moveTo(shadowImage.width(), shadowImage.height());
                QRectF f2(shadowImage.width() -  config.cornerRadius * 2,
                          shadowImage.height() -  config.cornerRadius * 2,
                          config.cornerRadius * 2,  config.cornerRadius * 2);
                path1.arcTo(f2, 270, 90);
                path1.lineTo(shadowImage.width(), shadowImage.height());
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

                shadowImage = QImage(pixel, pixel, QImage::Format_ARGB32_Premultiplied);
                shadowImage.fill(Qt::transparent);

                QPainter shadowPainter(&shadowImage);
                shadowPainter.setRenderHint(QPainter::Antialiasing, true);

                QRectF rectangle(config.shadowBlur, config.shadowBlur, innerPixel, innerPixel);
                QPainterPath path;
                path.addRoundedRect(rectangle, config.cornerRadius, config.cornerRadius);
                QPen pen(Qt::transparent);
                shadowPainter.setPen(pen);
                shadowPainter.fillPath(path, config.shadowColor);
                shadowPainter.drawPath(path);
                shadowPainter.end();

                if (config.shadowBlur > 0) {
                    qt_image_boxblur(shadowImage, qMax(1, qRound(config.shadowBlur / 2)), true);
                }
            }

            texture = new Texture(context->createTexture(shadowImage));
            texture->cacheKey = to_cache_key_key;
            texture->texture->setFiltering(QSGTexture::Nearest);
            texture->texture->setVerticalWrapMode(QSGTexture::ClampToEdge);
            texture->texture->setHorizontalWrapMode(QSGTexture::ClampToEdge);

            if (cache && !m_cache.contains(to_cache_key_key))
                m_cache[to_cache_key_key] = texture;
        }

        TextureData data(texture);
        return data;
    }

private:
    ShadowTextureCache() {

    }

    QHash<QString, Texture*> m_cache;
};

class DQuickShadowImagePrivate : public QQuickItemPrivate
{
    Q_DECLARE_PUBLIC(DQuickShadowImage)

public:
    DQuickShadowImagePrivate()
        : QQuickItemPrivate()
        , shadowColor(Qt::transparent)
        , isInner(false)
        , needUpdateShadow(false)
        , shadowTexture(nullptr)
    {

    }

    /**
     * @brief textureForShadow Call function on the rendering thread
     * @return
     */
    QSGTexture *ensureTextureForShadow()
    {
        // when doing geometry change animation, I don't want to draw shadows again,
        // resulting in performance degradation.
        if (needUpdateShadow || !shadowTexture) {
            shadowTexture = ShadowTextureCache::instance()->getShadowTexture(sceneGraphRenderContext(), shadowConfig(), cache, antialiasing);
        }
        needUpdateShadow = false;
        return shadowTexture ? shadowTexture->texture : nullptr;
    }

    bool hasCache()
    {
        return ShadowTextureCache::instance()->keyIsCache(shadowConfig());
    }

    void calculateRects(const QSize &sourceSize,
                        const QSizeF &targetSize,
                        qreal devicePixelRatio,
                        QRectF *targetRect,
                        QRectF *innerTargetRect,
                        QRectF *innerSourceRect,
                        QRectF *subSourceRect);
private:
    ShadowTextureCache::ShadowConfig shadowConfig()
    {
        Q_Q(DQuickShadowImage);
        ShadowTextureCache::ShadowConfig config;
        config.cornerRadius = q->calculateCornerRadius();
        config.shadowBlur = shadowBlur;
        config.shadowColor = shadowColor;
        config.inner = isInner;
        config.shapeType = q->shapeIsCircular() ? ShadowTextureCache::Circular : ShadowTextureCache::Rectangle;
        return config;
    }

public:
    qreal shadowBlur;
    QColor shadowColor;
    qreal cornerRadius;
    bool isInner;
    bool cache;
    std::atomic<bool> needUpdateShadow;
    ShadowTextureCache::TextureData shadowTexture;
};

DQUICK_END_NAMESPACE

#endif // DQUICKSHADOWIMAGE_P_H
