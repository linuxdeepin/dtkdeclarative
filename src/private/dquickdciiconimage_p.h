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
#include <DDciIconPalette>

DQUICK_BEGIN_NAMESPACE
DGUI_USE_NAMESPACE

class DQuickDciIconImageItem;
class DQuickIconAttached;
class DQuickDciIconImagePrivate;
class DQuickDciIconImage : public QQuickItem, DCORE_NAMESPACE::DObject
{
    Q_OBJECT
    D_DECLARE_PRIVATE(DQuickDciIconImage)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged FINAL)
    Q_PROPERTY(DQMLGlobalObject::ControlState mode READ mode WRITE setMode NOTIFY modeChanged FINAL)
    Q_PROPERTY(Theme theme READ theme WRITE setTheme NOTIFY themeChanged FINAL)
    Q_PROPERTY(DTK_GUI_NAMESPACE::DDciIconPalette palette READ palette WRITE setPalette NOTIFY paletteChanged)
    Q_PROPERTY(QSize sourceSize READ sourceSize WRITE setSourceSize NOTIFY sourceSizeChanged)
    Q_PROPERTY(bool mirror READ mirror WRITE setMirror NOTIFY mirrorChanged)

public:
    enum Theme {
        UnknowTheme = -1,
        Light = DDciIcon::Light,
        Dark = DDciIcon::Dark
    };

    Q_ENUM(Theme)
    explicit DQuickDciIconImage(QQuickItem *parent = nullptr);
    ~DQuickDciIconImage() override;

    QString name() const;
    void setName(const QString &name);

    DQMLGlobalObject::ControlState mode() const;
    void setMode(DQMLGlobalObject::ControlState mode);

    DQuickDciIconImage::Theme theme() const;
    void setTheme(DQuickDciIconImage::Theme theme);

    DDciIconPalette palette() const;
    void setPalette(const DDciIconPalette &palette);

    QSize sourceSize() const;
    void setSourceSize(const QSize &size);

    void setMirror(bool mirror);
    bool mirror() const;

    DQuickIconImage *imageItem() const;

    static bool isNull(const QString &iconName);
    static DQuickIconAttached *qmlAttachedProperties(QObject *object);

Q_SIGNALS:
    void nameChanged();
    void modeChanged();
    void themeChanged();
    void paletteChanged();
    void sourceSizeChanged();
    void mirrorChanged();

protected:
    void classBegin() override;
    void componentComplete() override;
};

class DQuickIconAttachedPrivate;
class DQuickIconAttached : public QObject, DCORE_NAMESPACE::DObject
{
    Q_OBJECT
    D_DECLARE_PRIVATE(DQuickIconAttached)
    Q_PROPERTY(DTK_QUICK_NAMESPACE::DQMLGlobalObject::ControlState mode READ mode WRITE setMode NOTIFY modeChanged)
    Q_PROPERTY(DTK_QUICK_NAMESPACE::DQuickDciIconImage::Theme theme READ theme WRITE setTheme NOTIFY themeChanged)
    Q_PROPERTY(DTK_GUI_NAMESPACE::DDciIconPalette palette READ palette WRITE setPalette NOTIFY paletteChanged)
public:
    explicit DQuickIconAttached(QQuickItem *parent);
    ~DQuickIconAttached();

    DQMLGlobalObject::ControlState mode() const;
    void setMode(DQMLGlobalObject::ControlState mode);

    DQuickDciIconImage::Theme theme() const;
    void setTheme(DQuickDciIconImage::Theme theme);

    DDciIconPalette palette() const;
    void setPalette(const DDciIconPalette &palette);

Q_SIGNALS:
    void modeChanged();
    void themeChanged();
    void paletteChanged();
};

DQUICK_END_NAMESPACE
QML_DECLARE_TYPEINFO(DTK_QUICK_NAMESPACE::DQuickDciIconImage, QML_HAS_ATTACHED_PROPERTIES)
#endif // DQUICKDCIICONIMAGE_P_H
