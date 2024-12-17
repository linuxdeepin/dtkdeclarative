// SPDX-FileCopyrightText: 2024 - 2025 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef DQUICKRADIUSEFFECT_P_H
#define DQUICKRADIUSEFFECT_P_H

#include "dquickradiuseffect.h"

#include <private/qquickitem_p.h>

DQUICK_BEGIN_NAMESPACE

class Q_DECL_HIDDEN DQuickRadiusEffectPrivate : public QQuickItemPrivate
{
    Q_DECLARE_PUBLIC(DQuickRadiusEffect)

public:
    DQuickRadiusEffectPrivate()
        : provider(nullptr)
        , texture(nullptr)
        , wrapMode(DQuickRadiusEffect::ClampToEdge)
        , sourceItem(nullptr)
        , textureSize(0, 0)
        , format(DQuickRadiusEffect::RGBA8)
        , samples(0)
        , live(true)
        , hideSource(false)
        , mipmap(false)
        , recursive(false)
        , grab(true)
        , textureMirroring(DQuickRadiusEffect::MirrorVertically)
        , radius(0)
        , isQSGPlaintexture(false)
        , isQSGDynamicTexture(false)
    {
    }

    ~DQuickRadiusEffectPrivate()
    {
    }

    struct ExtraData {
        ExtraData()
            : topLeftRadius(-1.),
            topRightRadius(-1.),
            bottomLeftRadius(-1.),
            bottomRightRadius(-1.)
        {
        }

        qreal topLeftRadius;
        qreal topRightRadius;
        qreal bottomLeftRadius;
        qreal bottomRightRadius;
    };

    mutable DQuickRadiusEffectTextureProvider *provider;
    QSGLayer *texture;
    DQuickRadiusEffect::WrapMode wrapMode;
    QQuickItem *sourceItem;
    QRectF sourceRect;
    QSize textureSize;
    DQuickRadiusEffect::Format format;
    int samples;
    uint live : 1;
    uint hideSource : 1;
    uint mipmap : 1;
    uint recursive : 1;
    uint grab : 1;
    uint textureMirroring : 2; // Stores TextureMirroring enum
    QLazilyAllocated<ExtraData> extraRectangle;
    qreal radius;
    bool isQSGPlaintexture;
    bool isQSGDynamicTexture;
};

DQUICK_END_NAMESPACE

#endif // DQUICKRADIUSEFFECT_P_P_H
