/*
 * Copyright (C) 2021 UnionTech Technology Co., Ltd.
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
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef DQUICKCONTROLPALETTE_P_H
#define DQUICKCONTROLPALETTE_P_H

#include <dtkdeclarative_global.h>
#include <DQMLGlobalObject>

#include <QColor>
#include <QObject>
#include <QQmlParserStatus>
#include <QQmlListProperty>
#include <QQmlEngine>
#include <QVector>

QT_BEGIN_NAMESPACE
class QQuickItem;
class QQuickWindow;
QT_END_NAMESPACE

DQUICK_BEGIN_NAMESPACE
class DQuickControlColor
{
    Q_GADGET
    Q_PROPERTY(QColor common READ common WRITE setCommon FINAL)
    Q_PROPERTY(QColor crystal READ crystal WRITE setCrystal FINAL)

public:
    DQuickControlColor();
    ~DQuickControlColor();
    DQuickControlColor(QColor *colors);
    DQuickControlColor(const DQuickControlColor &other)
        : colors(other.colors) {}
    DQuickControlColor(DQuickControlColor &&other)
        {colors = std::move(other.colors);}
    inline DQuickControlColor &operator =(const DQuickControlColor &other)
    {colors = other.colors; return *this;}

    const QColor &common() const;
    void setCommon(const QColor &newCommon);
    const QColor &crystal() const;
    void setCrystal(const QColor &newCrystal);

private:
    QColor *colors = nullptr;
};
DQUICK_END_NAMESPACE
Q_DECLARE_METATYPE(DTK_QUICK_NAMESPACE::DQuickControlColor)

DQUICK_BEGIN_NAMESPACE

class DQuickControlPalette : public QObject
{
    friend class DQuickControlColorSelector;
    Q_OBJECT
    Q_DISABLE_COPY(DQuickControlPalette)
    Q_PROPERTY(DTK_QUICK_NAMESPACE::DQuickControlColor normal READ normal WRITE set NOTIFY changed)
    Q_PROPERTY(DTK_QUICK_NAMESPACE::DQuickControlColor normalDark READ normalDark WRITE set NOTIFY changed)
    Q_PROPERTY(DTK_QUICK_NAMESPACE::DQuickControlColor hovered READ hovered WRITE set NOTIFY changed)
    Q_PROPERTY(DTK_QUICK_NAMESPACE::DQuickControlColor hoveredDark READ hoveredDark WRITE set NOTIFY changed)
    Q_PROPERTY(DTK_QUICK_NAMESPACE::DQuickControlColor pressed READ pressed WRITE set NOTIFY changed)
    Q_PROPERTY(DTK_QUICK_NAMESPACE::DQuickControlColor pressedDark READ pressedDark WRITE set NOTIFY changed)
    Q_PROPERTY(DTK_QUICK_NAMESPACE::DQuickControlColor disabled READ disabled WRITE set NOTIFY changed)
    Q_PROPERTY(DTK_QUICK_NAMESPACE::DQuickControlColor disabledDark READ disabledDark WRITE set NOTIFY changed)

public:
    enum ColorFamily {
        CommonColor = 0,
        CrystalColor = 1,
        ColorFamilyCount
    };
    Q_ENUM(ColorFamily)

    enum ColorGroup {
        Light = 0,
        Dark = 1,
        ThemeTypeCount,

        Normal = 0,
        Hovered = 1 * (ThemeTypeCount + ColorFamilyCount),
        Pressed = 2 * (ThemeTypeCount + ColorFamilyCount),
        Disabled = 3 * (ThemeTypeCount + ColorFamilyCount),
        ColorTypeCount = 4
    };

    explicit DQuickControlPalette(QObject *parent = nullptr);
    ~DQuickControlPalette();

    void set(const DQuickControlColor &) {
        Q_EMIT changed();
    }
    DQuickControlColor normal() {
        return DQuickControlColor(colors.data() + 0 * ColorFamilyCount);
    }
    DQuickControlColor normalDark() {
        return DQuickControlColor(colors.data() + 1 * ColorFamilyCount);
    }
    DQuickControlColor hovered() {
        return DQuickControlColor(colors.data() + 2 * ColorFamilyCount);
    }
    DQuickControlColor hoveredDark() {
        return DQuickControlColor(colors.data() + 3 * ColorFamilyCount);
    }
    DQuickControlColor pressed() {
        return DQuickControlColor(colors.data() + 4 * ColorFamilyCount);
    }
    DQuickControlColor pressedDark() {
        return DQuickControlColor(colors.data() + 5 * ColorFamilyCount);
    }
    DQuickControlColor disabled() {
        return DQuickControlColor(colors.data() + 6 * ColorFamilyCount);
    }
    DQuickControlColor disabledDark() {
        return DQuickControlColor(colors.data() + 7 * ColorFamilyCount);
    }

Q_SIGNALS:
    void changed();

public:
    QVector<QColor> colors;
};
DQUICK_END_NAMESPACE
QML_DECLARE_TYPE(DTK_QUICK_NAMESPACE::DQuickControlPalette)

DQUICK_BEGIN_NAMESPACE
class CustomMetaObject;
class DQuickControlColorSelector : public QObject, public QQmlParserStatus
{
    Q_OBJECT
    Q_DISABLE_COPY(DQuickControlColorSelector)
public:
    Q_PROPERTY(QQuickItem *control READ control WRITE setControl NOTIFY controlChanged)
    Q_PROPERTY(DTK_GUI_NAMESPACE::DGuiApplicationHelper::ColorType controlTheme READ controlTheme NOTIFY controlThemeChanged)
    Q_PROPERTY(DTK_QUICK_NAMESPACE::DQMLGlobalObject::ControlState controlState READ controlState NOTIFY controlStateChanged)
    Q_PROPERTY(bool hovered READ hovered WRITE setHovered RESET resetHovered NOTIFY hoveredChanged)
    Q_PROPERTY(bool pressed READ pressed WRITE setPressed RESET resetPressed NOTIFY pressedChanged)
    Q_PROPERTY(bool disabled READ disabled WRITE setDisabled RESET resetDisabled NOTIFY disabledChanged)
    Q_PROPERTY(bool inactived READ inactived WRITE setInactived RESET resetInactived NOTIFY inactivedChanged)
    Q_PROPERTY(QQmlListProperty<DTK_QUICK_NAMESPACE::DQuickControlPalette> palettes READ palettes NOTIFY palettesChanged FINAL)
    Q_CLASSINFO("DefaultProperty", "palettes")
    Q_INTERFACES(QQmlParserStatus)

    explicit DQuickControlColorSelector(QObject *parent = nullptr);
    ~DQuickControlColorSelector();

    static DQuickControlColorSelector *qmlAttachedProperties(QObject *object);

    QQuickItem *control() const;
    void setControl(QQuickItem *newControl);

    DQMLGlobalObject::ControlState controlState() const;
    DGuiApplicationHelper::ColorType controlTheme() const;

    bool hovered() const;
    void setHovered(bool newHovered);
    void resetHovered();

    bool pressed() const;
    void setPressed(bool newPressed);
    void resetPressed();

    bool disabled() const;
    void setDisabled(bool newDisabled);
    void resetDisabled();

    bool inactived() const;
    void setInactived(bool newInactived);
    void resetInactived();

    QQmlListProperty<DQuickControlPalette> palettes();

Q_SIGNALS:
    void controlThemeChanged();
    void controlStateChanged();
    void controlChanged();
    void hoveredChanged();
    void pressedChanged();
    void disabledChanged();
    void inactivedChanged();
    void palettesChanged();

private:
    void classBegin() override;
    void componentComplete() override;

    QColor getColorOf(const DQuickControlPalette *palette,
                      DGuiApplicationHelper::ColorType theme,
                      DQMLGlobalObject::ControlState state);
    void ensureMetaObject();
    void updatePropertyForPalette(const DQuickControlPalette *palette);
    void removePropertyForPalette(const DQuickControlPalette *palette);
    Q_SLOT void updateControlTheme();
    Q_SLOT void updateControlState();
    Q_SLOT void updateProperties();
    Q_SLOT void onPaletteChanged();
    Q_SLOT void onControlWindowChanged();

    void setControlTheme(DGuiApplicationHelper::ColorType theme);
    void setControlState(DQMLGlobalObject::ControlState state);

    static void palette_append(QQmlListProperty<DQuickControlPalette> *property, DQuickControlPalette *value);
    static int palette_count(QQmlListProperty<DQuickControlPalette> *property);
    static DQuickControlPalette *palette_at(QQmlListProperty<DQuickControlPalette> *property, int index);
    static void palette_clear(QQmlListProperty<DQuickControlPalette> *property);

    bool m_componentCompleted = true;
    QQuickItem *m_control = nullptr;
    QQuickWindow *m_controlWindow = nullptr;
    QList<DQuickControlPalette*> m_palettes;
    CustomMetaObject *m_metaObject = nullptr;
    DGuiApplicationHelper::ColorType m_controlTheme = DGuiApplicationHelper::LightType;
    DQMLGlobalObject::ControlState m_controlState = DQMLGlobalObject::NormalState;
    uint m_hovered:1;
    uint m_hoveredValueValid:1;
    uint m_pressed:1;
    uint m_pressedValueValid:1;
    uint m_disabled:1;
    uint m_disabledValueValid:1;
    uint m_inactived:1;
    uint m_inactivedValueValid:1;
};

DQUICK_END_NAMESPACE
QML_DECLARE_TYPE(DTK_QUICK_NAMESPACE::DQuickControlColorSelector)
QML_DECLARE_TYPEINFO(DTK_QUICK_NAMESPACE::DQuickControlColorSelector, QML_HAS_ATTACHED_PROPERTIES)

#endif // DQUICKCONTROLPALETTE_P_H
