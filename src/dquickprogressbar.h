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

#ifndef DQUICKPROGRESSBAR_H
#define DQUICKPROGRESSBAR_H

#include <QQuickItem>

class DQuickProgressBarPrivate;

class DQuickProgressBar : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)
    Q_PROPERTY(qreal radius READ radius WRITE setRadius NOTIFY radiusChanged)
    Q_PROPERTY(qreal value READ value WRITE setValue NOTIFY valueChanged)
    Q_PROPERTY(Qt::Orientation orientation READ orientation WRITE setOrientation)

public:
    explicit DQuickProgressBar(QQuickItem *parent = nullptr);

    QColor color() const;
    qreal radius() const;
    qreal value() const;
    Qt::Orientation orientation() const;

    void setRadius(qreal radius);
    void setColor(const QColor &color);
    void setValue(qreal value);
    void setOrientation(Qt::Orientation orientation);

Q_SIGNALS:
    void colorChanged(const QColor &color);
    void radiusChanged(qreal radius);
    void valueChanged(qreal value);

protected:
    QSGNode *updatePaintNode(QSGNode *oldNode, UpdatePaintNodeData *data) Q_DECL_OVERRIDE;
    DQuickProgressBar(DQuickProgressBarPrivate &dd, QQuickItem *parent);

private:
    Q_DISABLE_COPY(DQuickProgressBar)
    Q_DECLARE_PRIVATE(DQuickProgressBar)
};

#endif // DQUICKPROGRESSBAR_H
