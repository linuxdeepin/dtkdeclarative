// SPDX-FileCopyrightText: 2024 - 2025 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef DQUICKRADIUSEFFECT_H
#define DQUICKRADIUSEFFECT_H

#include <dtkdeclarative_global.h>

#include <private/qsgadaptationlayer_p.h>
#include <private/qsgcontext_p.h>
#include <private/qquickitemchangelistener_p.h>
#include <private/qlazilyallocated_p.h>
#include <private/qsgplaintexture_p.h>

#include <QQuickItem>
#include <QSGTextureProvider>

DQUICK_BEGIN_NAMESPACE

class DQuickRadiusEffectTextureProvider;
class DQuickRadiusEffectPrivate;

class DQuickRadiusEffect : public QQuickItem, public QQuickItemChangeListener
{
    Q_OBJECT
    Q_PROPERTY(qreal radius READ radius WRITE setRadius NOTIFY radiusChanged)
    Q_PROPERTY(qreal topLeftRadius READ topLeftRadius WRITE setTopLeftRadius NOTIFY topLeftRadiusChanged RESET resetTopLeftRadius FINAL)
    Q_PROPERTY(qreal topRightRadius READ topRightRadius WRITE setTopRightRadius NOTIFY topRightRadiusChanged RESET resetTopRightRadius FINAL)
    Q_PROPERTY(qreal bottomLeftRadius READ bottomLeftRadius WRITE setBottomLeftRadius NOTIFY bottomLeftRadiusChanged RESET resetBottomLeftRadius FINAL)
    Q_PROPERTY(qreal bottomRightRadius READ bottomRightRadius WRITE setBottomRightRadius NOTIFY bottomRightRadiusChanged RESET resetBottomRightRadius FINAL)
    Q_PROPERTY(WrapMode wrapMode READ wrapMode WRITE setWrapMode NOTIFY wrapModeChanged)
    Q_PROPERTY(QQuickItem *sourceItem READ sourceItem WRITE setSourceItem NOTIFY sourceItemChanged)
    Q_PROPERTY(QRectF sourceRect READ sourceRect WRITE setSourceRect NOTIFY sourceRectChanged)
    Q_PROPERTY(QSize textureSize READ textureSize WRITE setTextureSize NOTIFY textureSizeChanged)
    Q_PROPERTY(Format format READ format WRITE setFormat NOTIFY formatChanged)
    Q_PROPERTY(bool live READ live WRITE setLive NOTIFY liveChanged)
    Q_PROPERTY(bool hideSource READ hideSource WRITE setHideSource NOTIFY hideSourceChanged)
    Q_PROPERTY(bool mipmap READ mipmap WRITE setMipmap NOTIFY mipmapChanged)
    Q_PROPERTY(bool recursive READ recursive WRITE setRecursive NOTIFY recursiveChanged)
    Q_PROPERTY(TextureMirroring textureMirroring READ textureMirroring WRITE setTextureMirroring NOTIFY textureMirroringChanged REVISION(2, 6))
    Q_PROPERTY(int samples READ samples WRITE setSamples NOTIFY samplesChanged REVISION(2, 9))
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    QML_NAMED_ELEMENT(DRadiusEffect)
#endif

public:
    enum WrapMode {
        ClampToEdge,
        RepeatHorizontally,
        RepeatVertically,
        Repeat
    };
    Q_ENUM(WrapMode)

    enum Format {
        RGBA8 = 1,
        RGBA16F,
        RGBA32F,
        Alpha = RGBA8,
        RGB = RGBA8,
        RGBA = RGBA8
    };
    Q_ENUM(Format)

    enum TextureMirroring {
        NoMirroring        = 0x00,
        MirrorHorizontally = 0x01,
        MirrorVertically   = 0x02
    };
    Q_ENUM(TextureMirroring)

    DQuickRadiusEffect(QQuickItem *parent = nullptr);
    ~DQuickRadiusEffect() override;

    qreal radius() const;
    void setRadius(qreal radius);

    qreal topLeftRadius() const;
    void setTopLeftRadius(qreal radius);
    void resetTopLeftRadius();
    qreal topRightRadius() const;
    void setTopRightRadius(qreal radius);
    void resetTopRightRadius();
    qreal bottomLeftRadius() const;
    void setBottomLeftRadius(qreal radius);
    void resetBottomLeftRadius();
    qreal bottomRightRadius() const;
    void setBottomRightRadius(qreal radius);
    void resetBottomRightRadius();

    WrapMode wrapMode() const;
    void setWrapMode(WrapMode mode);

    QQuickItem *sourceItem() const;
    void setSourceItem(QQuickItem *item);

    QRectF sourceRect() const;
    void setSourceRect(const QRectF &rect);

    QSize textureSize() const;
    void setTextureSize(const QSize &size);

    Format format() const;
    void setFormat(Format format);

    bool live() const;
    void setLive(bool live);

    bool hideSource() const;
    void setHideSource(bool hide);

    bool mipmap() const;
    void setMipmap(bool enabled);

    bool recursive() const;
    void setRecursive(bool enabled);

    TextureMirroring textureMirroring() const;
    void setTextureMirroring(TextureMirroring mirroring);

    bool isTextureProvider() const override { return true; }
    QSGTextureProvider *textureProvider() const override;

    Q_INVOKABLE void scheduleUpdate();

    int samples() const;
    void setSamples(int count);

Q_SIGNALS:
    void wrapModeChanged();
    void sourceItemChanged();
    void sourceRectChanged();
    void textureSizeChanged();
    void formatChanged();
    void liveChanged();
    void hideSourceChanged();
    void mipmapChanged();
    void recursiveChanged();
    void textureMirroringChanged();
    void samplesChanged();

    void scheduledUpdateCompleted();

    void radiusChanged();
    void topLeftRadiusChanged();
    void topRightRadiusChanged();
    void bottomLeftRadiusChanged();
    void bottomRightRadiusChanged();

private Q_SLOTS:
    void sourceItemDestroyed(QObject *item);
    void invalidateSceneGraph();

protected:
    void releaseResources() override;
    QSGNode *updatePaintNode(QSGNode *, UpdatePaintNodeData *) override;
    void itemGeometryChanged(QQuickItem *item, QQuickGeometryChange change, const QRectF &) override;
    void itemChange(ItemChange change, const ItemChangeData &value) override;
    DQuickRadiusEffect(DQuickRadiusEffectPrivate &dd, QQuickItem *parent = nullptr);

    void maybeSetImplicitAntialiasing();
    void setImplicitAntialiasing(bool antialiasing);

private:
    void ensureTexture();

private:
    Q_DISABLE_COPY(DQuickRadiusEffect)
    Q_DECLARE_PRIVATE(DQuickRadiusEffect)
};

DQUICK_END_NAMESPACE

#endif // DQUICKRADIUSEFFECT_H
