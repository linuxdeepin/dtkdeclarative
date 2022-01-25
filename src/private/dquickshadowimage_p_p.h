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

#ifndef DQUICKSHADOWIMAGE_P_P_H
#define DQUICKSHADOWIMAGE_P_P_H

#include "dquickshadowimage_p.h"

#include <private/qquickitem_p.h>

#include <private/qquickitem_p.h>

#include <QString>
#include <QtMath>
#include <QFileInfo>
#include <QFutureWatcher>
#include <QtConcurrent>

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
        qreal spread = 0.0;
        qreal offsetX = 0.0;
        qreal offsetY = 0.0;

        bool operator==(ShadowConfig &other) {
            return (qFuzzyCompare(cornerRadius, other.cornerRadius)
                   && qFuzzyCompare(shadowBlur, other.shadowBlur)
                   && shadowColor == other.shadowColor
                   && inner == other.inner
                   && shapeType == other.shapeType
                   && qFuzzyCompare(spread, other.spread));
        }
        bool operator!=(ShadowConfig &other) {
            return !operator==(other);
        }
    };

    class Texture : public QSharedData {
    public:
        Texture(QImage *t)
            : texture(t)
        {}

        ~Texture() {
            ShadowTextureCache::instance()->m_cache.remove(cacheKey);
            delete texture;
        }

        QString cacheKey;
        QImage *texture;
    };

    typedef QExplicitlySharedDataPointer<Texture> TextureData;

    static ShadowTextureCache *instance()
    {
        static ShadowTextureCache *object = new ShadowTextureCache();
        return object;
    }

    QString buildShadowCacheKey(const ShadowConfig &config)
    {
        return QString("%1.%2.%3.%4.%5.%6.%7.%8").arg(config.cornerRadius)
                .arg(config.shadowBlur).arg(config.shadowColor.name(QColor::HexArgb))
                .arg(config.inner).arg(config.shapeType).arg(config.spread)
                .arg(config.offsetX).arg(config.offsetY);
    }

    QImage qt_image_convolute_filter(const QImage& src, const QVector<qreal>& weights, int radius = 0);

    void qt_image_boxblur(QImage& image, int radius, bool quality)
    {
        int passes = quality? 3: 1;
        int filterSize = 2 * radius + 1;
        for (int i = 0; i < passes; ++i)
            image = qt_image_convolute_filter(image, QVector<qreal>() << 1.0 / (filterSize * filterSize), radius);
    }

    bool keyIsCache(const ShadowConfig &config)
    {
        const QString &to_cache_key_key = buildShadowCacheKey(config);
        QString path = ":/dtk/declarative/shadow/" + to_cache_key_key + ".png";
        return m_cache.contains(to_cache_key_key) || QFileInfo::exists(path);
    }

    TextureData getShadowTexture(const ShadowConfig &config, bool cache = false, bool antialiasing = true);

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
        , cache(true)
        , shadowTexture(nullptr)
        , spread(0.0)
        , hollow(false)
        , offsetX(0.0)
        , offsetY(0.0)
        , cacheShadow(nullptr)
        , needUpdateHollow (false)
    {

    }

    ~DQuickShadowImagePrivate() override
    {
        delete cacheShadow;
    }

    /**
     * @brief textureForShadow Call function on the rendering thread
     * @return
     */
    QImage *ensureTextureForShadow()
    {
        // when doing geometry change animation, I don't want to draw shadows again,
        // resulting in performance degradation.
        auto newConfig = shadowConfig();
        if (shadowTexture && (newConfig == curConfig))
            return nullptr;

        curConfig = newConfig;
        if (ShadowTextureCache::instance()->keyIsCache(curConfig)) {
            shadowTexture = ShadowTextureCache::instance()->getShadowTexture(curConfig, cache, antialiasing);
            if (hollow && !isInner)
                needUpdateHollow = true;
        } else {
            QFutureWatcher<ShadowTextureCache::TextureData> *watcher = new QFutureWatcher<ShadowTextureCache::TextureData>();
            QObject::connect(watcher, &QFutureWatcher<QSGTexture *>::finished, q_func(), [watcher, this]{
                shadowTexture = watcher->result();
                if (hollow && !isInner)
                    needUpdateHollow = true;
                q_func()->update();
                watcher->deleteLater();
            });

            QFuture<ShadowTextureCache::TextureData> future = QtConcurrent::run(ShadowTextureCache::instance(), &ShadowTextureCache::getShadowTexture,
                                                                                curConfig, cache, antialiasing);
            watcher->setFuture(future);
        }
        return shadowTexture ? shadowTexture->texture : nullptr;
    }

    void updateCacheShadow(const QImage &image)
    {
        if (cacheShadow)
            delete cacheShadow;
        cacheShadow = sceneGraphRenderContext()->createTexture(image);
    }

    QImage reprocessHollowShadowTexture()
    {
        if (Q_LIKELY(!needUpdateHollow) || shadowColor.alpha() == 0)
            return QImage();
        // use deep copy to avoid polluting cached images
        QImage image = shadowTexture->texture->copy(shadowTexture->texture->rect());
        QPainter hollow(&image);
        hollow.setRenderHint(QPainter::Antialiasing, true);
        hollow.setCompositionMode(QPainter::CompositionMode_Clear);
        QPainterPath path;
        hollow.setPen(Qt::NoPen);
        qreal pixel, innerPixel;
        pixel = shadowTexture->texture->size().width();
        if (curConfig.shapeType == ShadowTextureCache::Rectangle) {
            innerPixel = pixel - curConfig.shadowBlur * 2.0;
        } else {
            innerPixel = pixel - curConfig.shadowBlur * 2.0;
        }

        QRectF rectangle(curConfig.shadowBlur - offsetX, curConfig.shadowBlur - offsetY,
                         innerPixel - 2, innerPixel - 2);
        path.addRoundedRect(rectangle, curConfig.cornerRadius, curConfig.cornerRadius);
        hollow.fillPath(path, Qt::red);
        hollow.end();

        needUpdateHollow = false;
        return image;
    }

    void calculateRects(const QSize &sourceSize,
                        const QSizeF &targetSize,
                        qreal devicePixelRatio,
                        QRectF *targetRect,
                        QRectF *innerTargetRect,
                        QRectF *innerSourceRect,
                        QRectF *subSourceRect);
private:
    inline bool shapeIsCircular() const
    {
        Q_Q(const DQuickShadowImage);
        if (isInner)
            return cornerRadius * 2 >= std::min({q->width(), q->height()});
        return cornerRadius * 2 >= std::min({q->width() - shadowBlur * 2.0, q->height() - shadowBlur * 2.0});
    }

    inline qreal calculateCornerRadius() const
    {
        Q_Q(const DQuickShadowImage);
        if (isInner)
            return std::min({cornerRadius, std::min({q->width() / 2.0, q->height() / 2.0})});
        return std::min({cornerRadius, std::min({(q->width() - 2.0 * shadowBlur) / 2.0,
                                                 (q->height() - 2.0 * shadowBlur) / 2.0})});
    }

    ShadowTextureCache::ShadowConfig shadowConfig() const
    {
        ShadowTextureCache::ShadowConfig config;
        config.cornerRadius = calculateCornerRadius();
        config.shadowBlur = shadowBlur;
        config.shadowColor = shadowColor;
        config.inner = isInner;
        config.shapeType = shapeIsCircular() ? ShadowTextureCache::Circular
                                             : ShadowTextureCache::Rectangle;
        if (isInner) {
            config.spread = spread;
            config.offsetX = offsetX;
            config.offsetY = offsetY;
        }
        return config;
    }

public:
    qreal shadowBlur;
    QColor shadowColor;
    qreal cornerRadius;
    bool isInner;
    bool cache;
    ShadowTextureCache::TextureData shadowTexture;
    qreal spread;
    bool hollow;
    qreal offsetX;
    qreal offsetY;
    QSGTexture *cacheShadow;
    bool needUpdateHollow;
    ShadowTextureCache::ShadowConfig curConfig;
};

DQUICK_END_NAMESPACE

#endif // DQUICKSHADOWIMAGE_P_P_H
