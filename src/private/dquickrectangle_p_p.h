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

#ifndef DQUICKRECTANGLE_P_P_H
#define DQUICKRECTANGLE_P_P_H

#include "dquickrectangle_p.h"
#include "dquickitemviewport_p.h"

#include <private/qquickitem_p.h>

DQUICK_BEGIN_NAMESPACE

class Q_DECL_HIDDEN DQuickRectanglePrivate : public QQuickItemPrivate
{
    Q_DECLARE_PUBLIC(DQuickRectangle)

public:
    DQuickRectanglePrivate()
        : QQuickItemPrivate ()
        , radius(0.0)
        , color(Qt::white)
    {

    }

    inline QSGTexture *textureForRadiusMask()
    {
        if (!maskTexture && radius > 0) {
            QQuickItemPrivate *d = QQuickItemPrivate::get(q_func());
            maskTexture = MaskTextureCache::instance()->getTexture(d->sceneGraphRenderContext(),
                                                                   radius * d->window->effectiveDevicePixelRatio(), true);
        }
        return maskTexture->texture;
    }

    qreal radius;
    QColor color;
    DQuickRectangle::Corners corners;
    MaskTextureCache::TextureData maskTexture;
};

DQUICK_END_NAMESPACE

#endif // DQUICKRECTANGLE_P_P_H
