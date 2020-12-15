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

#ifndef DQMLGLOBALOBJECT_H
#define DQMLGLOBALOBJECT_H
#include <QQuickWindow>
#include <DObject>

#include <dtkdeclarative_global.h>
#include <DWindowManagerHelper>
#include <DPlatformThemeProxy>
#include <DQuickWindow>

DGUI_USE_NAMESPACE

DQUICK_BEGIN_NAMESPACE

class DQMLGlobalObjectPrivate;
class DQMLGlobalObject : public QObject, public DTK_CORE_NAMESPACE::DObject
{
    Q_OBJECT

    Q_PROPERTY(bool hasBlurWindow READ hasBlurWindow NOTIFY hasBlurWindowChanged)
    Q_PROPERTY(bool hasComposite READ hasComposite NOTIFY hasCompositeChanged)
    Q_PROPERTY(bool hasNoTitlebar READ hasNoTitlebar NOTIFY hasNoTitlebarChanged)
    Q_PROPERTY(DTK_GUI_NAMESPACE::DWindowManagerHelper::WMName windowManagerName READ windowManagerName)
    Q_PROPERTY(QString windowManagerNameString READ windowManagerNameString)
    Q_PROPERTY(DPlatformThemeProxy *applicationTheme READ applicationTheme)
    Q_PROPERTY(DPlatformThemeProxy *systemTheme READ systemTheme)

public:
    explicit DQMLGlobalObject(QObject *parent = nullptr);
    ~DQMLGlobalObject() override;

    bool hasBlurWindow() const;
    bool hasComposite() const;
    bool hasNoTitlebar() const;

    DWindowManagerHelper::WMName windowManagerName() const;
    QString windowManagerNameString() const;

    DPlatformThemeProxy *applicationTheme() const;
    DPlatformThemeProxy *systemTheme() const;

Q_SIGNALS:
    void hasBlurWindowChanged();
    void hasCompositeChanged();
    void hasNoTitlebarChanged();

private:
    D_DECLARE_PRIVATE(DQMLGlobalObject)
};

DQUICK_END_NAMESPACE

#endif // DQMLGLOBALOBJECT_H
