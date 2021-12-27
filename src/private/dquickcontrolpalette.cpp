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
#include "dquickcontrolpalette_p.h"

#include <DGuiApplicationHelper>

#include <QPalette>
#include <QQuickItem>
#include <QQuickWindow>
#include <QDebug>

#include <private/qqmlopenmetaobject_p.h>
#include <private/qqmlglobal_p.h>

DGUI_USE_NAMESPACE
DQUICK_BEGIN_NAMESPACE

#if QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)
#define ThrowError(obj, message) \
    qmlEngine(obj)->throwError(message)
#else
#define ThrowError(obj, message) \
    qCritical() << message.toLocal8Bit()
#endif

DQuickControlColor::DQuickControlColor()
{

}

DQuickControlColor::DQuickControlColor(const QColor &color)
    : color(color)
{

}

DQuickControlColor::~DQuickControlColor()
{

}

DQuickControlColor::DQuickControlColor(QColor *colors)
    : colors(colors)
{

}

const QColor &DQuickControlColor::common() const
{
    Q_ASSERT(colors);
    return colors[DQuickControlPalette::CommonColor];
}

void DQuickControlColor::setCommon(const QColor &newCommon)
{
    Q_ASSERT(colors);
    colors[DQuickControlPalette::CommonColor] = newCommon;
}

const QColor &DQuickControlColor::crystal() const
{
    Q_ASSERT(colors);
    return colors[DQuickControlPalette::CrystalColor];
}

void DQuickControlColor::setCrystal(const QColor &newCrystal)
{
    Q_ASSERT(colors);
    colors[DQuickControlPalette::CrystalColor] = newCrystal;
}

DQuickControlPalette::DQuickControlPalette(QObject *parent)
    : QObject(parent)
    , colors(ThemeTypeCount * ColorTypeCount * ColorFamilyCount, QColor())
{

}

DQuickControlPalette::~DQuickControlPalette()
{

}

class Q_DECL_HIDDEN CustomMetaObject : public QQmlOpenMetaObject
{
public:
    CustomMetaObject(DQuickControlColorSelector *obj)
        : QQmlOpenMetaObject(obj, new QQmlOpenMetaObjectType(obj->metaObject(), qmlEngine(obj)))
    {

    }

    ~CustomMetaObject()
    {

    }

    inline DQuickControlColorSelector *owner() const {
        return qobject_cast<DQuickControlColorSelector*>(object());
    }

    QVariant propertyWriteValue(int id, const QVariant &v) override {
        if (v.canConvert<DQuickControlPalette*>()) {
            auto palette = v.value<DQuickControlPalette*>();
            const QMetaProperty p = property(id + propertyOffset());
            const QString pn = QString::fromLatin1(p.name());
            if (pn != palette->objectName()) {
                ThrowError(palette, QStringLiteral("Unable to assign Palette \"%1\" of objectName to property \"%2\"")
                           .arg(palette->objectName(), pn));
                return value(id);
            }
            const auto &pl = owner()->m_palettes;
            for (int i = pl.count() - 1; i >= 0; --i) {
                if (owner()->m_palettes.at(i)->objectName() == pn) {
                    owner()->palette_replace(i, palette, false);
                    const QColor c = owner()->getColorOf(palette, owner()->m_controlTheme, owner()->m_controlState);
                    return c;
                }
            }
        }
        return QQmlOpenMetaObject::propertyWriteValue(id, v);
    }
};

DQuickControlColorSelector::DQuickControlColorSelector(QObject *parent)
    : QObject(parent)
    , m_componentCompleted(true)
    , m_hovered(false)
    , m_hoveredValueValid(false)
    , m_pressed(false)
    , m_pressedValueValid(false)
    , m_disabled(false)
    , m_disabledValueValid(false)
    , m_inactived(false)
    , m_inactivedValueValid(false)
{

}

DQuickControlColorSelector::~DQuickControlColorSelector()
{

}

DQuickControlColorSelector *DQuickControlColorSelector::qmlAttachedProperties(QObject *object)
{
#if defined(QT_NAMESPACE)
#define NAMESPACE_STR1(NAME) #NAME"::QQuickControl"
#define NAMESPACE_STR(R) NAMESPACE_STR1(R)
#define ControlBaseClassName NAMESPACE_STR(QT_NAMESPACE)
#else
#define ControlBaseClassName "QQuickControl"
#endif
    DQuickControlColorSelector *colorSelector = nullptr;
    if (!object->inherits(ControlBaseClassName))
        return colorSelector;

    if (QQuickItem *control = qobject_cast<QQuickItem *>(object)) {
        colorSelector = new DQuickControlColorSelector(object);
        colorSelector->setControl(control);
    }

    return colorSelector;
}

QQuickItem *DQuickControlColorSelector::control() const
{
    return m_control;
}

void DQuickControlColorSelector::setControl(QQuickItem *newControl)
{
    Q_ASSERT(newControl);
    if (m_control == newControl)
        return;

    // Clean for old
    if (m_control) {
        m_control->disconnect(this);
        if (auto w = m_control->window())
            w->disconnect(this);
    }
    if (m_controlWindow) {
        m_controlWindow->disconnect(this);
        m_controlWindow = nullptr;
    }

    m_control = newControl;

    if (m_componentCompleted) {
        DQuickControlColorSelector::componentComplete();
    }

    if (m_metaObject)
        updateProperties();

    Q_EMIT controlChanged();
}

DGuiApplicationHelper::ColorType DQuickControlColorSelector::controlTheme() const
{
    return m_controlTheme;
}

DQMLGlobalObject::ControlState DQuickControlColorSelector::controlState() const
{
    return m_controlState;
}

void DQuickControlColorSelector::setControlTheme(DGuiApplicationHelper::ColorType theme)
{
    if (m_controlTheme == theme)
        return;

    m_controlTheme = theme;
    Q_EMIT controlThemeChanged();
    updateProperties();
}

void DQuickControlColorSelector::setControlState(DQMLGlobalObject::ControlState state)
{
    if (m_controlState == state)
        return;

    m_controlState = state;
    Q_EMIT controlStateChanged();
    updateProperties();
}

bool DQuickControlColorSelector::hovered() const
{
    return m_hovered;
}

void DQuickControlColorSelector::setHovered(bool newHovered)
{
    m_hoveredValueValid = true;
    if (m_hovered == newHovered)
        return;
    m_hovered = newHovered;
    Q_EMIT hoveredChanged();
}

void DQuickControlColorSelector::resetHovered()
{
    setHovered(false);
    m_hoveredValueValid = false;
}

bool DQuickControlColorSelector::pressed() const
{
    return m_pressed;
}

void DQuickControlColorSelector::setPressed(bool newPressed)
{
    m_pressedValueValid = true;
    if (m_pressed == newPressed)
        return;
    m_pressed = newPressed;
    Q_EMIT pressedChanged();
}

void DQuickControlColorSelector::resetPressed()
{
    setPressed(false);
    m_pressedValueValid = false;
}

bool DQuickControlColorSelector::disabled() const
{
    return m_disabled;
}

void DQuickControlColorSelector::setDisabled(bool newDisabled)
{
    m_disabledValueValid = true;
    if (m_disabled == newDisabled)
        return;
    m_disabled = newDisabled;
    Q_EMIT disabledChanged();
}

void DQuickControlColorSelector::resetDisabled()
{
    setDisabled(false);
    m_disabledValueValid = false;
}

bool DQuickControlColorSelector::inactived() const
{
    return m_inactived;
}

void DQuickControlColorSelector::setInactived(bool newInactived)
{
    m_inactivedValueValid = true;
    if (m_inactived == newInactived)
        return;
    m_inactived = newInactived;
    Q_EMIT inactivedChanged();
}

void DQuickControlColorSelector::resetInactived()
{
    setInactived(false);
    m_inactivedValueValid = false;
}

void DQuickControlColorSelector::palette_append(QQmlListProperty<DQuickControlPalette> *property, DQuickControlPalette *value) {
    if (!value)
        return;
    auto that = qobject_cast<DQuickControlColorSelector*>(property->object);
    if (value->objectName().isEmpty())
        ThrowError(that, QStringLiteral("Must objectName for %1").arg(reinterpret_cast<quintptr>(value)));
    if (!value->parent())
        value->setParent(that);
    that->m_palettes.append(value);
    that->ensureMetaObject();
    that->updatePropertyForPalette(value);
    connect(value, &DQuickControlPalette::changed,
            that, std::bind(&DQuickControlColorSelector::updatePropertyForPalette, that, value));
    Q_EMIT that->palettesChanged();
}

int DQuickControlColorSelector::palette_count(QQmlListProperty<DQuickControlPalette> *property) {
    auto that = qobject_cast<DQuickControlColorSelector*>(property->object);
    return that->m_palettes.count();
}

DQuickControlPalette *DQuickControlColorSelector::palette_at(QQmlListProperty<DQuickControlPalette> *property, int index) {
    auto that = qobject_cast<DQuickControlColorSelector*>(property->object);
    return that->m_palettes.at(index);
}

void DQuickControlColorSelector::palette_clear(QQmlListProperty<DQuickControlPalette> *property) {
    auto that = qobject_cast<DQuickControlColorSelector*>(property->object);
    that->m_palettes.clear();
    Q_EMIT that->palettesChanged();
}

void DQuickControlColorSelector::palette_replace(int index, DQuickControlPalette *newValue, bool updateProperty)
{
    if (newValue->objectName().isEmpty())
        ThrowError(this, QString("Must objectName for 0x%1").arg(reinterpret_cast<quintptr>(newValue)));
    if (!newValue->parent())
        newValue->setParent(this);
    m_palettes.replace(index, newValue);
    if (updateProperty) {
        ensureMetaObject();
        updatePropertyForPalette(newValue);
    }
    Q_EMIT palettesChanged();
}

void DQuickControlColorSelector::palette_replace(QQmlListProperty<DQuickControlPalette> *property, int index,
                                                 DQuickControlPalette *newValue)
{
    auto that = qobject_cast<DQuickControlColorSelector*>(property->object);
    that->palette_replace(index, newValue, true);
}

void DQuickControlColorSelector::palette_remove_last(QQmlListProperty<DQuickControlPalette> *property)
{
    auto that = qobject_cast<DQuickControlColorSelector*>(property->object);
    that->m_palettes.removeLast();
    Q_EMIT that->palettesChanged();
}

QQmlListProperty<DQuickControlPalette> DQuickControlColorSelector::palettes()
{
    return QQmlListProperty<DQuickControlPalette>(this, this, palette_append, palette_count,
                                                  palette_at, palette_clear);
}

void DQuickControlColorSelector::classBegin()
{
    m_componentCompleted = false;
}

void DQuickControlColorSelector::componentComplete()
{
    ensureMetaObject();

    m_componentCompleted = true;
    if (m_control) {
        connect(m_control, SIGNAL(paletteChanged()), this, SLOT(updateControlTheme()));
        connect(m_control, SIGNAL(hoveredChanged()), this, SLOT(updateControlState()));
        connect(m_control, SIGNAL(pressedChanged()), this, SLOT(updateControlState()));
        connect(m_control, &QQuickItem::enabledChanged, this, &DQuickControlColorSelector::updateControlState);
        connect(m_control, &QQuickItem::windowChanged, this, &DQuickControlColorSelector::onControlWindowChanged);
        onControlWindowChanged();
    }
}

static inline QColor getColor(const DQuickControlPalette *palette, int themeIndex, int stateIndex) {
    QColor color = palette->colors.at(themeIndex + stateIndex);
    if (!color.isValid()) {
        // fallback to normal state
        color = palette->colors.at(themeIndex);
    }

    return color;
}

QColor DQuickControlColorSelector::getColorOf(const DQuickControlPalette *palette,
                                              DGuiApplicationHelper::ColorType theme,
                                              DQMLGlobalObject::ControlState state)
{
    QColor targetColor;

    int themeIndex = DQuickControlPalette::Light;
    if (theme == DGuiApplicationHelper::DarkType) {
        themeIndex = DQuickControlPalette::Dark;
    }

    // TODO: do DQuickControlPalette::ColorFamily, add the colorFamily property
    // to the window with Attached Properties.
    /* eg:
     * ApplicationWindow {
     *     D.Palette.colorFamily: D.Palette.CrystalColor
     * }
     */

    int stateIndex = DQuickControlPalette::Normal;
    bool disabled = m_disabledValueValid ? m_disabled : (state == DQMLGlobalObject::DisabledState);
    if (disabled) {
        targetColor = palette->colors.at(themeIndex + DQuickControlPalette::Disabled);
        if (targetColor.isValid()) // Don't process the disabled's color, should direct uses it.
            return targetColor;
        // fallback to normal color
    } else if (m_pressedValueValid ? m_pressed : (state == DQMLGlobalObject::PressedState)) {
        stateIndex = DQuickControlPalette::Pressed;
    } else if (m_hoveredValueValid ? m_hovered : (state == DQMLGlobalObject::HoveredState)) {
        stateIndex = DQuickControlPalette::Hovered;
    }

    targetColor = getColor(palette, themeIndex, stateIndex);
    if (!targetColor.isValid() && theme == DGuiApplicationHelper::DarkType) {
        // create the dark color from the light theme
        targetColor = getColor(palette, DQuickControlPalette::Light, stateIndex);
        // inverse the color to dark
        int r, g, b, a;
        targetColor.getRgb(&r, &g, &b, &a);
        targetColor = QColor(255 - r, 255 - g, 255 - b, a);
    }

    if (!targetColor.isValid() || !m_control || disabled)
        return targetColor;

    const QPalette pa = qvariant_cast<QPalette>(m_control->property("palette"));
    const QColor windowColor = pa.color(QPalette::Window);
    if (!windowColor.isValid())
        return targetColor;

    QColor inactive_mask_color = windowColor;

    if (theme == DGuiApplicationHelper::DarkType) {
        inactive_mask_color.setAlphaF(0.6);
    } else {
        inactive_mask_color.setAlphaF(0.4);
    }

    if ((m_inactivedValueValid ? m_inactived : (m_control->window() && !m_control->window()->isActive()))
            && DGuiApplicationHelper::testAttribute(DGuiApplicationHelper::Attribute::UseInactiveColorGroup)) {
        targetColor = DGuiApplicationHelper::blendColor(targetColor, inactive_mask_color);
    }

    return targetColor;
}

void DQuickControlColorSelector::ensureMetaObject()
{
    if (m_metaObject)
        return;

    m_metaObject = new CustomMetaObject(this);
    m_metaObject->setCached(true);

    for (auto palette : qAsConst(m_palettes)) {
        updatePropertyForPalette(palette);
    }
}

void DQuickControlColorSelector::updatePropertyForPalette(const DQuickControlPalette *palette)
{
    const auto id = palette->objectName();
    Q_ASSERT(!id.isEmpty());
    if (!id.isEmpty()) {
        const QColor color = getColorOf(palette, m_controlTheme, m_controlState);
        m_metaObject->setValue(id.toLatin1(), color);
    }
}

void DQuickControlColorSelector::removePropertyForPalette(const DQuickControlPalette *palette)
{
    const auto id = palette->objectName();
    if (!id.isEmpty()) {
        m_metaObject->setValue(id.toLatin1(), QVariant());
    }
}

void DQuickControlColorSelector::updateControlTheme()
{
    if (!m_control)
        return;

    const QPalette pa = qvariant_cast<QPalette>(m_control->property("palette"));
    const QColor windowColor = pa.color(QPalette::Window);

    if (!windowColor.isValid())
        return;

    const auto themeType = DGuiApplicationHelper::toColorType(windowColor);
    setControlTheme(themeType);
}

void DQuickControlColorSelector::updateControlState()
{
    if (!m_control)
        return;

    DQMLGlobalObject::ControlState state = DQMLGlobalObject::NormalState;
    if (!m_control->isEnabled()) {
        state = DQMLGlobalObject::DisabledState;
    } else if (m_control->property("pressed").toBool()) {
        state = DQMLGlobalObject::PressedState;
    } else if (m_control->property("hovered").toBool()) {
        state = DQMLGlobalObject::HoveredState;
    }

    setControlState(state);
}

void DQuickControlColorSelector::updateProperties()
{
    for (auto palette : qAsConst(m_palettes))
        updatePropertyForPalette(palette);
}

void DQuickControlColorSelector::onPaletteChanged()
{
    auto palette = qobject_cast<DQuickControlPalette*>(sender());
    Q_ASSERT(palette && m_palettes.contains(palette));
    updatePropertyForPalette(palette);
}

void DQuickControlColorSelector::onControlWindowChanged()
{
    if (m_controlWindow) {
        m_controlWindow->disconnect(this);
    }
    m_controlWindow = m_control->window();
    if (m_controlWindow) {
        connect(m_controlWindow, &QQuickWindow::activeChanged,
                this, &DQuickControlColorSelector::updateProperties);
    }
}

DQUICK_END_NAMESPACE
