/*
 * Copyright (C) 2020 ~ 2020 Uniontech Technology Co., Ltd.
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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef DQUICKICONLABEL_P_P_H
#define DQUICKICONLABEL_P_P_H

#include "dquickiconlabel_p.h"

#include <private/qquickitem_p.h>

DQUICK_BEGIN_NAMESPACE

class DQuickIconLabelPrivate : public QQuickItemPrivate, public QQuickItemChangeListener
{
    Q_DECLARE_PUBLIC(DQuickIconLabel)

public:
    bool hasIcon() const;
    bool hasText() const;

    bool createImage();
    bool destroyImage();
    bool updateImage();
    void syncImage();
    void updateOrSyncImage();

    bool createLabel();
    bool destroyLabel();
    bool updateLabel();
    void syncLabel();
    void updateOrSyncLabel();

    void updateImplicitSize();
    void layout();

    void watchChanges(QQuickItem *item);
    void unwatchChanges(QQuickItem *item);
    void setPositioningDirty();

    bool isLeftToRight() const;

    void itemImplicitWidthChanged(QQuickItem *) override;
    void itemImplicitHeightChanged(QQuickItem *) override;
    void itemDestroyed(QQuickItem *item) override;

    bool mirrored = false;
    DQuickIconLabel::Display display = DQuickIconLabel::TextBesideIcon;
    Qt::Alignment alignment = Qt::AlignCenter;
    qreal spacing = 0;
    qreal topPadding = 0;
    qreal leftPadding = 0;
    qreal rightPadding = 0;
    qreal bottomPadding = 0;
    QFont font;
    QColor color;
    QString text;
    QString iconName;
    QColor iconColor;
    DQuickIconImage *image = nullptr;
    QQuickText *label = nullptr;
};

DQUICK_END_NAMESPACE

#endif // DQUICKICONLABEL_P_P_H
