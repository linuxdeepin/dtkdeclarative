/*
 * Copyright (C) 2022 UnionTech Technology Co., Ltd.
 *
 * Author:     yeshanshan <yeshanshan@uniontech.com>
 *
 * Maintainer: yeshanshan <yeshanshan@uniontech.com>
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

#pragma once

#include <dtkdeclarative_global.h>

#include <QQmlComponent>
#include <QQuickItem>

DQUICK_BEGIN_NAMESPACE

class MessageManager;
class FloatingMessageContainer : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QQuickItem *panel READ panel WRITE setPanel)
    Q_PROPERTY(QVariant message READ message WRITE setMessage NOTIFY messageChanged)
    Q_PROPERTY(int duration READ duration WRITE setDuration NOTIFY durationChanged)
    Q_CLASSINFO("DefaultProperty", "panel")
public:
    explicit FloatingMessageContainer(QObject *parent = nullptr);

    QVariant message() const;
    void setMessage(const QVariant &message);
    QQuickItem *panel() const;
    void setPanel(QQuickItem *panel);
    QString messageId() const;
    void setMessageId(const QString &msgId);
    int duration() const;
    void setDuration(int duration);

public Q_SLOT:
    void close();

Q_SIGNALS:
    void messageChanged();
    void durationChanged();

private:
    friend MessageManager;

    QQuickItem *m_panel = nullptr;
    QVariant m_message;
    QString m_msgId;
    int m_duration = 4000;
};

class MessageManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QQmlComponent *delegate READ delegate WRITE setDelegate)
    Q_PROPERTY(QQuickItem *layout READ layout WRITE setLayout)
    Q_PROPERTY(int capacity READ capacity WRITE setCapacity)
    Q_PROPERTY(int count READ count NOTIFY countChanged)

public:
    explicit MessageManager(QQuickWindow *parent = nullptr);

    QQuickWindow *window() const;
    QQmlComponent *delegate() const;
    void setDelegate(QQmlComponent *delegate);
    QQuickItem *layout() const;
    void setLayout(QQuickItem * layout);
    int capacity() const;
    void setCapacity(int capacity);
    int count() const;
    QList<FloatingMessageContainer *> messages(const QString &msgId) const;

    static MessageManager *qmlAttachedProperties(QObject *object);

public Q_SLOTS:
    bool sendMessage(const QString &content, const QString &iconName = QString(), int duration = 4000, const QString &msgId = QString());
    bool sendMessage(QQmlComponent *delegate, const QVariant &message, int duration = 4000, const QString &msgId = QString());
    void close(DTK_QUICK_NAMESPACE::FloatingMessageContainer *message);
    void close(const QString &msgId);

Q_SIGNALS:
    void countChanged();

protected:
    virtual void timerEvent(QTimerEvent *e) override;

private:
    void ensureLayout();
    void ensureDelegate();
    bool reachedUpperLimit() const;
    FloatingMessageContainer *replaceMessage(const QString &msgId);
    void stackBeforeMessage(QQuickItem *message);
    FloatingMessageContainer *beginCreateMessage(QQmlComponent *component);
    void endCreateMessage(QQmlComponent *component, FloatingMessageContainer *container);

private:
    QQmlComponent *m_delegate;
    QQuickItem *m_layout;
    QList<QPair<int, FloatingMessageContainer *>> m_timers;
    int m_capacity = 3;
};

DQUICK_END_NAMESPACE

QML_DECLARE_TYPEINFO(DTK_QUICK_NAMESPACE::MessageManager, QML_HAS_ATTACHED_PROPERTIES)
