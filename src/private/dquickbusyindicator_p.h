/*
 * Copyright (C) 2020 ~ 2021 Deepin Technology Co., Ltd.
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

#ifndef DQUICKBUSYINDICATOR_P_H
#define DQUICKBUSYINDICATOR_P_H

#include <dtkdeclarative_global.h>

#include <QtGui/qcolor.h>
#include <QtQuick/qquickitem.h>
#include <QtQuick/private/qsgadaptationlayer_p.h>

DQUICK_BEGIN_NAMESPACE

class DQuickBusyIndicator;
class DQuickBusyIndicatorNode : public QObject, public QSGTransformNode
{
    Q_OBJECT

public:
    DQuickBusyIndicatorNode(DQuickBusyIndicator *item);

    void setSpinning(bool spinning);
    void sync(QQuickItem *item);

public Q_SLOTS:
    void maybeRotate();
    void maybeUpdate();
    void setFillColor(const QColor &color);

private:
    int updateIndicatorColors(const QColor &fill);

private:
    bool m_spinning;
    float m_rotation;
    QPointF m_center;
    QQuickWindow *m_window;
    QList<QList<QColor>> m_indicatorColors;
};

class DQuickBusyIndicator : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(QColor fillColor READ fillColor WRITE setFillColor NOTIFY fillColorChanged)
    Q_PROPERTY(bool running READ isRunning WRITE setRunning)

public:
    explicit DQuickBusyIndicator(QQuickItem *parent = nullptr);

    QColor fillColor() const;
    bool isRunning() const;

public Q_SLOTS:
    void setFillColor(const QColor &color);
    void setRunning(bool running);

protected:
    void itemChange(ItemChange change, const ItemChangeData &data) override;
    QSGNode *updatePaintNode(QSGNode *oldNode, UpdatePaintNodeData *) override;

Q_SIGNALS:
    void fillColorChanged();

private:
    QColor m_fillColor;
    bool m_isRunning;
    bool m_fillColorIsChanged;
};

DQUICK_END_NAMESPACE

QML_DECLARE_TYPE(DTK_QUICK_NAMESPACE::DQuickBusyIndicator)

#endif // DQUICKBUSYINDICATOR_P_H
