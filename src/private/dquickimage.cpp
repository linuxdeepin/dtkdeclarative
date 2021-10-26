/*
 * Copyright (C) 2020 ~ 2021 Uniontech Software Technology Co.,Ltd.
 *
 * Author:     xiaoyaobing <xiaoyaobing@uniontech.com>
 *
 * Maintainer: xiaoyaobing <xiaoyaobing@uniontech.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "dquickimage.h"
#include "dquickimage_p.h"
#include "scenegraph/qsgradiusimagenode.h"

#include <QtMath>

/**
 * @brief DQuickImage::DQuickImage DQuickImage is used to draw pictures with rounded corners and make pictures inverted
 * DImage {
 *     source: "qrc:/res/battery.svg"
 *     radius: 30
 *     sourceSize: Qt.size(128, 128)
 *     specifyColor: Qt.rgba(1, 0, 0, 1)
 *     renderTpye: DImage.AutoRender
 *  }
 * @param parent
 */
DQuickImage::DQuickImage(QQuickItem *parent)
    : QQuickImage(*(new DQuickImagePrivate), parent)
{
    setFlag(ItemHasContents, true);
}

QColor DQuickImage::specifyColor() const
{
    Q_D(const DQuickImage);

    return d->specifyColor;
}

qreal DQuickImage::radius() const
{
    Q_D(const DQuickImage);

    return d->radius;
}

DQuickImage::RenderType DQuickImage::renderTpye() const
{
    Q_D(const DQuickImage);

    return d->renderType;
}

/**
 * @brief DQuickImage::setRadius This property holds the corner radius used to draw a image.
 * If radius is non-zero, the image will be painted as a rounded image, otherwise it will be painted as a normal image.
 * The same radius is used by all 4 corners; there is currently no way to specify different radii for different corners.
 * range [0, ∞),However, when displaying, the maximum fillet radius can only be half the height or half the width
 * @param radius The default is 0
 */
void DQuickImage::setRadius(qreal radius)
{
    Q_D(DQuickImage);

    if (qFuzzyCompare(d->radius, radius))
        return;

    d->radius = radius;
    Q_EMIT radiusChanged(radius);
    update();
}

/**
 * @brief DQuickImage::setSpecifyColor When the RenderType is RenderWithColor, the picture is
 * drawn according to this setting color
 * @param color
 */
void DQuickImage::setSpecifyColor(const QColor &color)
{
    Q_D(DQuickImage);

    d->specifyColor = color;
    Q_EMIT specifyColorChanged(color);
}

/**
 * @brief DQuickImage::setRenderType Set the rendering type. If it is AutoRender, the original color of the image
 * will be displayed. If it is RenderWithColor, it will be drawn according to the specifyColor color combined with
 * the alpha of the image texture.
 * @param type The default is AutoRender
 */
void DQuickImage::setRenderType(DQuickImage::RenderType type)
{
    Q_D(DQuickImage);

    if (d->renderType == type)
        return;

    d->renderType = type;
    update();
}

DQuickImage::DQuickImage(DQuickImagePrivate &dd, QQuickItem *parent)
    : QQuickImage (dd, parent)
{

}

QSGNode *DQuickImage::updatePaintNode(QSGNode *oldNode, QQuickItem::UpdatePaintNodeData *)
{
    Q_D(DQuickImage);

    QSGTexture *texture = d->sceneGraphRenderContext()->textureForFactory(d->pix.textureFactory(), window());

    if (!texture || width() <= 0 || height() <= 0) {
        delete oldNode;
        return nullptr;
    }

    QSGradiusImageNode *node = static_cast<QSGradiusImageNode *>(oldNode);
    if (!node) {
        d->pixmapChanged = true;
        node = new QSGradiusImageNode;
    }

    QRectF targetRect;
    QRectF sourceRect;
    QSGTexture::WrapMode hWrap = QSGTexture::ClampToEdge;
    QSGTexture::WrapMode vWrap = QSGTexture::ClampToEdge;

    qreal pixWidth = (d->fillMode == PreserveAspectFit) ? d->paintedWidth : d->pix.width() / d->devicePixelRatio;
    qreal pixHeight = (d->fillMode == PreserveAspectFit) ? d->paintedHeight :  d->pix.height() / d->devicePixelRatio;

    int xOffset = 0;
    if (d->hAlign == QQuickImage::AlignHCenter)
        xOffset = (width() - pixWidth) / 2;
    else if (d->hAlign == QQuickImage::AlignRight)
        xOffset = qCeil(width() - pixWidth);

    int yOffset = 0;
    if (d->vAlign == QQuickImage::AlignVCenter)
        yOffset = (height() - pixHeight) / 2;
    else if (d->vAlign == QQuickImage::AlignBottom)
        yOffset = qCeil(height() - pixHeight);

    switch (d->fillMode) {
    case Stretch:
        targetRect = QRectF(0, 0, width(), height());
        sourceRect = d->pix.rect();
        break;

    case PreserveAspectFit:
        targetRect = QRectF(xOffset, yOffset, d->paintedWidth, d->paintedHeight);
        sourceRect = d->pix.rect();
        break;

    case PreserveAspectCrop: {
        targetRect = QRectF(0, 0, width(), height());
        qreal wscale = width() / qreal(d->pix.width());
        qreal hscale = height() / qreal(d->pix.height());

        if (wscale > hscale) {
            int src = (hscale / wscale) * qreal(d->pix.height());
            int y = 0;
            if (d->vAlign == QQuickImage::AlignVCenter)
                y = qCeil((d->pix.height() - src) / 2.);
            else if (d->vAlign == QQuickImage::AlignBottom)
                y = qCeil(d->pix.height() - src);
            sourceRect = QRectF(0, y, d->pix.width(), src);

        } else {
            int src = (wscale / hscale) * qreal(d->pix.width());
            int x = 0;
            if (d->hAlign == QQuickImage::AlignHCenter)
                x = qCeil((d->pix.width() - src) / 2.);
            else if (d->hAlign == QQuickImage::AlignRight)
                x = qCeil(d->pix.width() - src);
            sourceRect = QRectF(x, 0, src, d->pix.height());
        }
    }
        break;

    case Tile:
        targetRect = QRectF(0, 0, width(), height());
        sourceRect = QRectF(-xOffset, -yOffset, width(), height());
        hWrap = QSGTexture::Repeat;
        vWrap = QSGTexture::Repeat;
        break;

    case TileHorizontally:
        targetRect = QRectF(0, 0, width(), height());
        sourceRect = QRectF(-xOffset, 0, width(), d->pix.height());
        hWrap = QSGTexture::Repeat;
        break;

    case TileVertically:
        targetRect = QRectF(0, 0, width(), height());
        sourceRect = QRectF(0, -yOffset, d->pix.width(), height());
        vWrap = QSGTexture::Repeat;
        break;

    case Pad:
        qreal w = qMin(qreal(pixWidth), width());
        qreal h = qMin(qreal(pixHeight), height());
        qreal x = (pixWidth > width()) ? -xOffset : 0;
        qreal y = (pixHeight > height()) ? -yOffset : 0;
        targetRect = QRectF(x + xOffset, y + yOffset, w, h);
        sourceRect = QRectF(x, y, w, h);
        break;
    }

    qreal nsWidth = (hWrap == QSGTexture::Repeat || d->fillMode == Pad) ? d->pix.width() / d->devicePixelRatio : d->pix.width();
    qreal nsHeight = (vWrap == QSGTexture::Repeat || d->fillMode == Pad) ? d->pix.height() / d->devicePixelRatio : d->pix.height();
    QRectF nsrect(sourceRect.x() / nsWidth,
                  sourceRect.y() / nsHeight,
                  sourceRect.width() / nsWidth,
                  sourceRect.height() / nsHeight);

    if (targetRect.isEmpty()
            || !qt_is_finite(targetRect.width()) || !qt_is_finite(targetRect.height())
            || nsrect.isEmpty()
            || !qt_is_finite(nsrect.width()) || !qt_is_finite(nsrect.height())) {
        delete node;
        return nullptr;
    }

    if (d->pixmapChanged) {
        if (texture->isAtlasTexture() && (hWrap == QSGTexture::Repeat || vWrap == QSGTexture::Repeat || d->mipmap))
            node->setTexture(texture->removedFromAtlas());
        else
            node->setTexture(texture);
        d->pixmapChanged = false;
    }

    node->setRadius(d->radius);
    node->setColor(d->specifyColor);
    node->setRenderType(d->renderType);
    node->setMipmapFiltering(d->mipmap ? QSGTexture::Linear : QSGTexture::None);
    node->setHorizontalWrapMode(hWrap);
    node->setVerticalWrapMode(vWrap);
    node->setFiltering(d->smooth ? QSGTexture::Linear : QSGTexture::Nearest);

    node->setTargetRect(targetRect);
    node->setInnerTargetRect(targetRect);
    node->setSubSourceRect(nsrect);
    node->setMirror(d->mirror);
    node->setAntialiasing(d->antialiasing);
    node->update();

    return node;
}

