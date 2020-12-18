/*
 * Copyright (C) 2020 ~ 2020 Deepin Technology Co., Ltd.
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

#ifndef DQUICKICONFINDER_H
#define DQUICKICONFINDER_H

#include <QUrl>
#include <QIcon>
#include <DObject>
#include <QString>
#include <QObject>

#include <dtkdeclarative_global.h>

DQUICK_BEGIN_NAMESPACE

class DQuickIconFinderPrivate;
class DQuickIconFinder : public QObject, public DTK_CORE_NAMESPACE::DObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(State state READ state WRITE setState NOTIFY stateChanged)
    Q_PROPERTY(Mode mode READ mode WRITE setMode NOTIFY modeChanged)
    Q_PROPERTY(QSize size READ size WRITE setSize NOTIFY sizeChanged)
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)
    Q_PROPERTY(QUrl url READ url NOTIFY urlChanged)

public:
    enum class Mode { Normal, Disabled, Active, Selected };
    enum class State { On, Off };

    Q_ENUM(Mode)
    Q_ENUM(State)

    explicit DQuickIconFinder(QObject *parent = Q_NULLPTR);
    ~DQuickIconFinder() override;

    QString name() const;
    DQuickIconFinder::State state() const;
    DQuickIconFinder::Mode mode() const;
    QSize size() const;
    QColor color() const;
    QUrl url() const;
    QIcon icon() const;
    QPixmap pixmap() const;

    static DQuickIconFinder *queryToObject(const QString &query);

public Q_SLOTS:
    void setName(const QString &name);
    void setState(const DQuickIconFinder::State state);
    void setMode(const DQuickIconFinder::Mode mode);
    void setSize(const QSize &size);
    void setColor(const QColor &color);
    void updateIcon(QByteArray iconThemeName);

Q_SIGNALS:
    void nameChanged();
    void stateChanged();
    void modeChanged();
    void sizeChanged();
    void colorChanged();
    void urlChanged();

private:
    D_DECLARE_PRIVATE(DQuickIconFinder)
};

DQUICK_END_NAMESPACE

#endif // DQUICKICONFINDER_H
