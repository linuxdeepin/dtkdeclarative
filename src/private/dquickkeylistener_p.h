/*
 * Copyright (C) 2022 UnionTech Technology Co., Ltd.
 *
 * Author:     Chen Bin <chenbin@uniontech.com>
 *
 * Maintainer: Chen Bin <chenbin@uniontech.com>
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

#ifndef DQUICKKEYLISTENER_P_H
#define DQUICKKEYLISTENER_P_H

#include <dtkdeclarative_global.h>

#include <DObject>
#include <QObject>

class QQuickItem;
DQUICK_BEGIN_NAMESPACE

class DQuickKeyListenerPrivate;
class DQuickKeyListener : public QObject, DCORE_NAMESPACE::DObject
{
    Q_OBJECT
    D_DECLARE_PRIVATE(DQuickKeyListener)
    Q_PROPERTY(QQuickItem *target READ target WRITE setTarget NOTIFY targetChanged)
    Q_PROPERTY(QStringList keys READ keys WRITE setKeys NOTIFY keysChanged)
    Q_PROPERTY(int maxKeyCount READ maxKeyCount WRITE setMaxKeyCount NOTIFY maxKeyCountChanged)

public:
    explicit DQuickKeyListener(QObject *parent = nullptr);

    QQuickItem *target() const;
    void setTarget(QQuickItem *item);

    const QStringList &keys() const;
    void setKeys(const QStringList &keys);

    int maxKeyCount() const;
    void setMaxKeyCount(int count);

    Q_INVOKABLE void clearKeys();

Q_SIGNALS:
    void targetChanged();
    void keysChanged();
    void maxKeyCountChanged();

protected:
    bool eventFilter(QObject *watched, QEvent *event) override;
};

DQUICK_END_NAMESPACE

#endif // DQUICKKEYLISTENER_P_H
