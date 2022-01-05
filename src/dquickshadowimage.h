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

#ifndef DQUICKSHADOWIMAGE_H
#define DQUICKSHADOWIMAGE_H

#include <dtkdeclarative_global.h>

#include <QQuickItem>
#include <QImage>

DQUICK_BEGIN_NAMESPACE

class DQuickShadowImagePrivate;
class Q_DECL_EXPORT DQuickShadowImage : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(bool cache READ cache WRITE setCache NOTIFY cacheChanged)
    Q_PROPERTY(qreal shadowBlur READ shadowBlur WRITE setShadowBlur NOTIFY shadowBlurChanged)
    Q_PROPERTY(QColor shadowColor READ shadowColor WRITE setShadowColor NOTIFY shadowColorChanged)
    Q_PROPERTY(bool isInner READ isInner WRITE setIsInner NOTIFY isInnerChanged)
    Q_PROPERTY(qreal cornerRadius READ cornerRadius WRITE setCornerRadius NOTIFY cornerRadiusChanded)

public:
    explicit DQuickShadowImage(QQuickItem *parent = nullptr);

    bool cache();
    void setCache(bool cache);

    qreal shadowBlur() const;
    void setShadowBlur(qreal blur);

    QColor shadowColor() const;
    void setShadowColor(const QColor &color);

    bool isInner();
    void setIsInner(bool inner);

    qreal cornerRadius();
    void setCornerRadius(qreal radius);

Q_SIGNALS:
    void cacheChanged();
    void shadowBlurChanged();
    void shadowColorChanged();
    void isInnerChanged();
    void cornerRadiusChanded();

protected:
    QSGNode *updatePaintNode(QSGNode *, UpdatePaintNodeData *) override;

protected:
    bool shapeIsCircular();
    qreal calculateCornerRadius();
    DQuickShadowImage(DQuickShadowImagePrivate &dd, QQuickItem *parent = nullptr);

private:
    Q_DISABLE_COPY(DQuickShadowImage)
    Q_DECLARE_PRIVATE(DQuickShadowImage)
};

DQUICK_END_NAMESPACE

#endif // DQUICKSHADOWIMAGE_H
