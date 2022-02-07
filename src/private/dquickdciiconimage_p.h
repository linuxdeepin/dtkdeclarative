/*
 * Copyright (C) 2021 UnionTech Technology Co., Ltd.
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
#ifndef DQUICKDCIICONIMAGE_P_H
#define DQUICKDCIICONIMAGE_P_H

#include "dquickiconimage_p.h"
#include "dqmlglobalobject_p.h"

#include <dtkdeclarative_global.h>
#include <DObject>
#include <DDciIcon>

DQUICK_BEGIN_NAMESPACE
DGUI_USE_NAMESPACE

class DQuickIconAttached;
class DQuickDciIconImagePrivate;
class DQuickDciIconImage : public DQuickIconImage
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(DQuickDciIconImage)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged FINAL)
    Q_PROPERTY(Type type READ type WRITE setType NOTIFY typeChanged FINAL)
    Q_PROPERTY(DQMLGlobalObject::ControlState mode READ mode WRITE setMode NOTIFY modeChanged FINAL)
    Q_PROPERTY(Theme theme READ theme WRITE setTheme NOTIFY themeChanged FINAL)
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged FINAL)

public:
    enum Type {
        UnknowType = -1,
        TextType = DDciIcon::TextType,
        ActionType = DDciIcon::ActionType,
        IconType = DDciIcon::IconType
    };

    enum Theme {
        UnknowTheme = -1,
        Light = DDciIcon::Light,
        Dark = DDciIcon::Dark
    };

    Q_ENUM(Type)
    Q_ENUM(Theme)
    explicit DQuickDciIconImage(QQuickItem *parent = nullptr);
    ~DQuickDciIconImage() override;

    QString name() const;
    void setName(const QString &name);

    Type type() const;
    void setType(Type type);

    DQMLGlobalObject::ControlState mode() const;
    void setMode(DQMLGlobalObject::ControlState mode);

    Theme theme() const;
    void setTheme(Theme theme);

    QColor color() const;
    void setColor(QColor color);

    bool isNull(const QString &iconName = QString()) const;
    static DQuickIconAttached *qmlAttachedProperties(QObject *object);

Q_SIGNALS:
    void nameChanged();
    void typeChanged();
    void modeChanged();
    void themeChanged();
    void colorChanged();
};

class DQuickIconAttachedPrivate;
class DQuickIconAttached : public QObject, DCORE_NAMESPACE::DObject
{
    Q_OBJECT
    D_DECLARE_PRIVATE(DQuickIconAttached)
    Q_PROPERTY(DTK_QUICK_NAMESPACE::DQuickDciIconImage::Type type READ type WRITE setType NOTIFY typeChanged)
    Q_PROPERTY(DTK_QUICK_NAMESPACE::DQMLGlobalObject::ControlState mode READ mode WRITE setMode NOTIFY modeChanged)
    Q_PROPERTY(DTK_QUICK_NAMESPACE::DQuickDciIconImage::Theme theme READ theme WRITE setTheme NOTIFY themeChanged)
public:
    explicit DQuickIconAttached(QQuickItem *parent);
    ~DQuickIconAttached();

    DQuickDciIconImage::Type type() const;
    void setType(DQuickDciIconImage::Type type);

    DQMLGlobalObject::ControlState mode() const;
    void setMode(DQMLGlobalObject::ControlState mode);

    DQuickDciIconImage::Theme theme() const;
    void setTheme(DQuickDciIconImage::Theme theme);

Q_SIGNALS:
    void typeChanged();
    void modeChanged();
    void themeChanged();
};

DQUICK_END_NAMESPACE
QML_DECLARE_TYPEINFO(DTK_QUICK_NAMESPACE::DQuickDciIconImage, QML_HAS_ATTACHED_PROPERTIES)
#endif // DQUICKDCIICONIMAGE_P_H
