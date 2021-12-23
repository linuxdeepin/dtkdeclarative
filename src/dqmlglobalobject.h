/*
 * Copyright (C) 2020 ~ 2021 deepin Technology Co., Ltd.
 *
 * Author:     JiDe Zhang <zhangjide@deepin.org>
 *
 * Maintainer: JiDe Zhang <zhangjide@deepin.org>
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

class DQuickDciIcon;
class DQMLGlobalObjectPrivate;
class DQMLGlobalObject : public QObject, public DTK_CORE_NAMESPACE::DObject
{
    Q_OBJECT

    Q_PROPERTY(bool hasBlurWindow READ hasBlurWindow NOTIFY hasBlurWindowChanged)
    Q_PROPERTY(bool hasComposite READ hasComposite NOTIFY hasCompositeChanged)
    Q_PROPERTY(bool hasNoTitlebar READ hasNoTitlebar NOTIFY hasNoTitlebarChanged)
    Q_PROPERTY(bool isSoftwareRender READ isSoftwareRender FINAL CONSTANT)
    Q_PROPERTY(DTK_GUI_NAMESPACE::DWindowManagerHelper::WMName windowManagerName READ windowManagerName)
    Q_PROPERTY(DTK_GUI_NAMESPACE::DGuiApplicationHelper::ColorType themeType READ themeType NOTIFY themeTypeChanged)
    Q_PROPERTY(QString windowManagerNameString READ windowManagerNameString)
    Q_PROPERTY(DPlatformThemeProxy *platformTheme READ platformTheme FINAL CONSTANT)
    Q_PROPERTY(DTK_GUI_NAMESPACE::DFontManager *fontManager READ fontManager FINAL CONSTANT)
    Q_PROPERTY(QPalette palette READ palette NOTIFY paletteChanged)
    Q_PROPERTY(QPalette inactivePalette READ inactivePalette NOTIFY inactivePaletteChanged)
    Q_PROPERTY(QString deepinDistributionOrgLogo READ deepinDistributionOrgLogo CONSTANT)
    Q_PROPERTY(QString deepinWebsiteName READ deepinWebsiteName CONSTANT)
    Q_PROPERTY(QString deepinWebsiteLink READ deepinWebsiteLink CONSTANT)

public:
    explicit DQMLGlobalObject(QObject *parent = nullptr);
    ~DQMLGlobalObject() override;

    enum ControlState {
        NormalState,
        HoveredState,
        PressedState,
        DisabledState
    };
    Q_ENUM(ControlState)

    enum ZOrder {
        BottomOrder = -99,
        BelowOrder = -1,
        NormalOrder = 0,
        AboveOrder = 1,
        TopOrder = 99
    };
    Q_ENUM(ZOrder)

    bool hasBlurWindow() const;
    bool hasComposite() const;
    bool hasNoTitlebar() const;
    bool isSoftwareRender() const;

    DWindowManagerHelper::WMName windowManagerName() const;
    QString windowManagerNameString() const;

    DGuiApplicationHelper::ColorType themeType() const;

    DPlatformThemeProxy *platformTheme() const;
    DFontManager *fontManager() const;

    QPalette palette() const;
    QPalette inactivePalette() const;

    Q_INVOKABLE QColor adjustColor(const QColor &base, qint8 hueFloat, qint8 saturationFloat, qint8 lightnessFloat,
                                   qint8 redFloat, qint8 greenFloat, qint8 blueFloat, qint8 alphaFloat);
    Q_INVOKABLE QColor blendColor(const QColor &substrate, const QColor &superstratum);

    Q_INVOKABLE DTK_GUI_NAMESPACE::DGuiApplicationHelper::ColorType toColorType(const QColor &color);
    Q_INVOKABLE QColor selectColor(const QColor &windowColor, const QColor &light, const QColor &dark);

    QString deepinWebsiteName() const;
    QString deepinWebsiteLink() const;
    QString deepinDistributionOrgLogo() const;

    Q_INVOKABLE QPoint cursorPosition() const;

    Q_INVOKABLE DTK_QUICK_NAMESPACE::DQuickDciIcon makeIcon(const QJSValue &qicon, const QJSValue &iconExtra);

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
