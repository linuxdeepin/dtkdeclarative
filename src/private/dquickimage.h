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

#ifndef DQUICKIMAGE_H
#define DQUICKIMAGE_H

#ifndef emit
#define emit Q_EMIT
#include <private/qquickimage_p.h>
#undef emit
#else
#include <private/qquickimage_p.h>
#endif

class DQuickImagePrivate;

class DQuickImage : public QQuickImage
{
    Q_OBJECT
    Q_ENUMS(RenderType)
    Q_PROPERTY(QColor specifyColor READ specifyColor WRITE setSpecifyColor NOTIFY specifyColorChanged)
    Q_PROPERTY(qreal radius READ radius WRITE setRadius NOTIFY radiusChanged)
    Q_PROPERTY(RenderType renderTpye READ renderTpye WRITE setRenderType NOTIFY renderTypeChanged)
public:
    enum RenderType {
        AutoRender,
        RenderWithColor
    };

public:
    DQuickImage(QQuickItem *parent = nullptr);

    QColor specifyColor() const;
    qreal radius() const;
    RenderType renderTpye() const;

    void setRadius(qreal radius);
    void setSpecifyColor(const QColor &color);
    void setRenderType(RenderType type);

Q_SIGNALS:
    void radiusChanged(qreal radius);
    void specifyColorChanged(const QColor &color);
    void renderTypeChanged(RenderType type);

protected:
    DQuickImage(DQuickImagePrivate &dd, QQuickItem *parent);
    QSGNode *updatePaintNode(QSGNode *, UpdatePaintNodeData *) override;

private:
    Q_DISABLE_COPY(DQuickImage)
    Q_DECLARE_PRIVATE(DQuickImage)
};

QML_DECLARE_TYPE(DQuickImage)
#endif // DQUICKIMAGE_H
