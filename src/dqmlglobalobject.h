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
#include <DGuiApplicationHelper>

DGUI_BEGIN_NAMESPACE
class DFontManager;
DGUI_END_NAMESPACE

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
    Q_PROPERTY(DTK_GUI_NAMESPACE::DGuiApplicationHelper::ColorType themeType READ themeType NOTIFY themeTypeChanged)
    Q_PROPERTY(QString windowManagerNameString READ windowManagerNameString)
    Q_PROPERTY(DPlatformThemeProxy *platformTheme READ platformTheme FINAL CONSTANT)
    Q_PROPERTY(DTK_GUI_NAMESPACE::DFontManager *fontManager READ fontManager FINAL CONSTANT)
    Q_PROPERTY(QPalette palette READ palette NOTIFY paletteChanged)
    Q_PROPERTY(QPalette inactivePalette READ inactivePalette NOTIFY inactivePaletteChanged)

public:
    explicit DQMLGlobalObject(QObject *parent = nullptr);
    ~DQMLGlobalObject() override;

    bool hasBlurWindow() const;
    bool hasComposite() const;
    bool hasNoTitlebar() const;

    DWindowManagerHelper::WMName windowManagerName() const;
    QString windowManagerNameString() const;

    DGuiApplicationHelper::ColorType themeType() const;

    DPlatformThemeProxy *platformTheme() const;
    DFontManager *fontManager() const;

    DPalette palette() const;
    DPalette inactivePalette() const;

Q_SIGNALS:
    void hasBlurWindowChanged();
    void hasCompositeChanged();
    void hasNoTitlebarChanged();
    void paletteChanged();
    void inactivePaletteChanged();
    void themeTypeChanged(DGuiApplicationHelper::ColorType themeType);

private:
    D_DECLARE_PRIVATE(DQMLGlobalObject)
    D_PRIVATE_SLOT(void _q_onPaletteChanged())
};

DQUICK_END_NAMESPACE

#endif // DQMLGLOBALOBJECT_H
