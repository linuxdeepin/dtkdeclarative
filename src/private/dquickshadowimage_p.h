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
#include <QtMath>
#include <QFileInfo>

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
                .arg(config.shadowBlur).arg(config.shadowColor.name(QColor::HexArgb)).arg(config.inner).arg(config.shapeType);
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

    TextureData getShadowTexture(QSGRenderContext *context, const ShadowConfig &config,
                                 bool cache = false, bool antialiasing = true);

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
        , needUpdateShadow(false)
        , shadowTexture(nullptr)
        , spread(0.0)
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
    qreal spread;
};

DQUICK_END_NAMESPACE

#endif // DQUICKSHADOWIMAGE_P_H
