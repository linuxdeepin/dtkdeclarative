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

#ifndef DQUICKBEHINDWINDOWBLUR_P_H
#define DQUICKBEHINDWINDOWBLUR_P_H

#include <DPlatformHandle>

#include <dtkdeclarative_global.h>

#include <QQuickItem>

DQUICK_BEGIN_NAMESPACE

class DQuickWindowAttached;
class DQuickBehindWindowBlurPrivate;
class DQuickBehindWindowBlur : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(qreal cornerRadius READ cornerRadius WRITE setCornerRadius NOTIFY cornerRadiusChanged)
    Q_PROPERTY(QColor blendColor READ blendColor WRITE setBlendColor NOTIFY blendColorChanged)
    Q_PROPERTY(bool valid READ valid NOTIFY validChanged)

public:
    explicit DQuickBehindWindowBlur(QQuickItem *parent = nullptr);
    ~DQuickBehindWindowBlur() override;

    qreal cornerRadius() const;
    void setCornerRadius(qreal newRadius);

    const QColor &blendColor() const;
    void setBlendColor(const QColor &newBlendColor);

    bool valid() const;

Q_SIGNALS:
    void cornerRadiusChanged();
    void blendColorChanged();
    void validChanged();

protected:
    QSGNode *updatePaintNode(QSGNode *, UpdatePaintNodeData *) override;
    void itemChange(ItemChange change, const ItemChangeData &value);
    void componentComplete() override;

private:
    void setWindowAttached(DQuickWindowAttached *wa);

private:
    Q_DISABLE_COPY(DQuickBehindWindowBlur)
    Q_DECLARE_PRIVATE(DQuickBehindWindowBlur)
    Q_PRIVATE_SLOT(d_func(), void _q_updateBlurArea())
    friend class DQuickWindowAttachedPrivate;
    friend class DSGBlendNode;
};

DQUICK_END_NAMESPACE

#endif // DQUICKBEHINDWINDOWBLUR_P_H
