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

#ifndef DQUICKOPACITYMASK_P_H
#define DQUICKOPACITYMASK_P_H

#include <dtkdeclarative_global.h>

#include <QQuickItem>

DQUICK_BEGIN_NAMESPACE

class DQuickOpacityMaskPrivate;
class DQuickOpacityMask : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(bool invert READ invert WRITE setInvert NOTIFY invertChanged)
    Q_PROPERTY(QQuickItem *source READ source WRITE setSource NOTIFY sourceChanged)
    Q_PROPERTY(QQuickItem *maskSource READ maskSource WRITE setMaskSource NOTIFY maskSourceChanged)

public:
    explicit DQuickOpacityMask(QQuickItem *parent = nullptr);

    bool invert();
    void setInvert(bool invert);

    QQuickItem *source() const;
    void setSource(QQuickItem *item);

    QQuickItem *maskSource() const;
    void setMaskSource(QQuickItem *item);

Q_SIGNALS:
    void invertChanged();
    void sourceChanged();
    void maskSourceChanged();

protected:
    QSGNode *updatePaintNode(QSGNode *oldNode, UpdatePaintNodeData *);

protected:
    DQuickOpacityMask(DQuickOpacityMaskPrivate &dd, QQuickItem *parent = nullptr);

private:
    Q_DISABLE_COPY(DQuickOpacityMask)
    Q_DECLARE_PRIVATE(DQuickOpacityMask)
};

DQUICK_END_NAMESPACE

#endif // DQUICKOPACITYMASK_P_H
