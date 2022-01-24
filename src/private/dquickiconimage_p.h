/*
 * Copyright (C) 2020 Uniontech Technology Co., Ltd.
 *
 * Author:     zccrs <zccrs@live.com>
 *
 * Maintainer: zccrs <zhangjide@uniontech.com>
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
#ifndef DQUICKICONIMAGE_H
#define DQUICKICONIMAGE_H

#include <dtkdeclarative_global.h>

#include <QIcon>
#ifndef emit
#define emit Q_EMIT
#include <private/qquickimage_p.h>
#undef emit
#else
#include <private/qquickimage_p.h>
#endif

DQUICK_BEGIN_NAMESPACE

class DQuickIconImagePrivate;
class DQuickIconImage : public QQuickImage
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged FINAL)
    Q_PROPERTY(Mode mode READ mode WRITE setMode NOTIFY modeChanged FINAL)
    Q_PROPERTY(State state READ state WRITE setState NOTIFY stateChanged FINAL)
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged FINAL)

public:
    enum class Mode {
        Invalid = QIcon::Normal - 1,
        Normal = QIcon::Normal,
        Disabled = QIcon::Disabled,
        Active = QIcon::Active,
        Selected = QIcon::Selected
    };
    enum class State {
        On = QIcon::On,
        Off = QIcon::Off
    };

    Q_ENUM(Mode)
    Q_ENUM(State)

    explicit DQuickIconImage(QQuickItem *parent = nullptr);
    ~DQuickIconImage();

    void componentComplete() override;

    QString name() const;
    State state() const;
    Mode mode() const;
    QColor color() const;

public Q_SLOTS:
    void setName(const QString &name);
    void setState(State state);
    void setMode(Mode mode);
    void setColor(const QColor &color);

Q_SIGNALS:
    void nameChanged();
    void stateChanged();
    void modeChanged();
    void colorChanged();

protected:
    DQuickIconImage(DQuickIconImagePrivate &dd, QQuickItem *parent);
    void itemChange(ItemChange change, const ItemChangeData &value) override;
    void pixmapChange() override;

private:
    Q_DISABLE_COPY(DQuickIconImage)
    Q_DECLARE_PRIVATE(DQuickIconImage)
    Q_PRIVATE_SLOT(d_func(), void maybeUpdateUrl())
};

DQUICK_END_NAMESPACE

#endif // DQUICKICONIMAGE_H
