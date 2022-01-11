/*
 * Copyright (C) 2020 ~ 2022 UnionTech Technology Co., Ltd.
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
#include <private/qmetaobjectbuilder_p.h>

DGUI_USE_NAMESPACE
DQUICK_BEGIN_NAMESPACE

#if QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)
#define ThrowError(obj, message) {\
    auto e = qmlEngine(obj);\
    if (e) e->throwError(message);\
    else qCritical().noquote() << message.toLocal8Bit();\
}
#else
#define ThrowError(obj, message) \
    qCritical().noquote() << message.toLocal8Bit()
#endif

#define METAPROPERTYCHANGEFUNC "resolveMetaPropertyChanged()"
#define COLORPROPERTYCHANGEFUNC "notifyColorPropertyChanged()"

static inline bool _d_isControlItem(QQuickItem *item) {
#if defined(QT_NAMESPACE)
#define CONTROL_BASE_NAMESPACE_STR1(NAME) #NAME"::QQuickControl"
#define CONTROL_BASE_NAMESPACE_STR(R) CONTROL_BASE_NAMESPACE_STR1(R)
#define ControlBaseClassName CONTROL_BASE_NAMESPACE_STR(QT_NAMESPACE)
#else
#define ControlBaseClassName "QQuickControl"
#endif

    return item->inherits(ControlBaseClassName);
}

static inline bool _d_isWindowRootItem(QQuickItem *item) {
#if defined(QT_NAMESPACE)
#define ROOT_ITEM_NAMESPACE_STR1(NAME) #NAME"::QQuickRootItem"
#define ROOT_ITEM_NAMESPACE_STR(R) ROOT_ITEM_NAMESPACE_STR1(R)
#define RootItemClassName ROOT_ITEM_NAMESPACE_STR(QT_NAMESPACE)
#else
#define RootItemClassName "QQuickRootItem"
#endif

    return item->inherits(RootItemClassName);
}

static QMetaProperty findMetaPropertyFromSignalIndex(const QObject *obj, int signal_index) {
    QMetaProperty itemProperty;
    if (signal_index < 0)
        return itemProperty;

    if (!obj)
        return itemProperty;

    auto itemMetaObject = obj->metaObject();
    for (int i = 0; i < itemMetaObject->propertyCount(); ++i) {
        itemProperty = itemMetaObject->property(i);
        if (itemProperty.isValid() && itemProperty.hasNotifySignal()) {
            int propertySignalIndex = itemProperty.notifySignalIndex();
            if (signal_index == propertySignalIndex)
                break;
        }
    }

    return itemProperty;
}

DQuickControlColor::DQuickControlColor()
{

}

DQuickControlColor::DQuickControlColor(const QColor &color)
    : data(new QColor(color))
    , isSingleColor(true)
{

}

DQuickControlColor::DQuickControlColor(QColor *colors)
    : data(colors)
    , isSingleColor(false)
{

}

DQuickControlColor::~DQuickControlColor()
{
    if (isSingleColor) {
        delete data;
    }
}

const QColor &DQuickControlColor::common() const
{
    Q_ASSERT(data && !isSingleColor);
    return data[DQuickControlPalette::CommonColor];
}

void DQuickControlColor::setCommon(const QColor &newCommon)
{
    Q_ASSERT(data && !isSingleColor);
    if (common() == newCommon)
        return;
    changed =  true;
    data[DQuickControlPalette::CommonColor] = newCommon;
}

const QColor &DQuickControlColor::crystal() const
{
    Q_ASSERT(data && !isSingleColor);
    return data[DQuickControlPalette::CrystalColor];
}

void DQuickControlColor::setCrystal(const QColor &newCrystal)
{
    Q_ASSERT(data && !isSingleColor);
    if (crystal() == newCrystal)
        return;
    changed = true;
    data[DQuickControlPalette::CrystalColor] = newCrystal;
}

DQuickControlPalette::DQuickControlPalette(QObject *parent)
    : QObject(parent)
    , colors(ThemeTypeCount * ColorTypeCount * ColorFamilyCount, QColor())
{

}

DQuickControlPalette::~DQuickControlPalette()
{

}

bool DQuickControlPalette::enabled() const
{
    return m_enabled;
}

void DQuickControlPalette::setEnabled(bool newEnabled)
{
    if (m_enabled == newEnabled)
        return;
    m_enabled = newEnabled;
    Q_EMIT enabledChanged();
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

    QVariant writePaletteValue(int id, DQuickControlPalette *palette) {
        const QMetaProperty p = property(id + propertyOffset());
        /* ###(Chen Bin)
         * When you modify the value of a palette in QML,
         * if the control does not already have a palette
         * for the property (perhaps for its QQuickControl
         * item), a new one is created for it.
         */
        owner()->setPalette(p.name(), palette);
        int slotIndex = owner()->metaObject()->indexOfSlot(METAPROPERTYCHANGEFUNC);
        if (slotIndex != -1) {
            QMetaObject::disconnect(owner()->parent(), p.notifySignalIndex(), owner(), slotIndex);
        }
        const QColor c = owner()->getColorOf(p.name(), owner()->m_state.data());
        return c;
    }

    QVariant propertyWriteValue(int id, const QVariant &v) override {
        if (v.canConvert<DQuickControlPalette*>()) {
            auto palette = v.value<DQuickControlPalette*>();
            return writePaletteValue(id, palette);
        }
        return QQmlOpenMetaObject::propertyWriteValue(id, v);
    }

    int metaCall(QObject *o, QMetaObject::Call _c, int _id, void **_a) override {
        if (_c == QMetaObject::ResetProperty) {
            auto ownerObject = owner()->parent();
            const QByteArray &proName = name((_id - propertyOffset()));
            int itemPropertyIndex = ownerObject->metaObject()->indexOfProperty(proName);
            if (itemPropertyIndex == -1) {
                int palIndex  = owner()->indexOfPalette(proName);
                if (palIndex != -1)
                    owner()->m_palettes.removeAt(palIndex);
                owner()->updatePropertyFromName(proName);
            } else {
                owner()->updatePaletteFromMetaProperty(ownerObject->metaObject()->property(itemPropertyIndex), ownerObject);
            }
        }
        return QQmlOpenMetaObject::metaCall(o, _c, _id, _a);
    }

    void propertyCreated(int id, QMetaPropertyBuilder &builder) override {
        builder.setResettable(true);
        if (builder.hasNotifySignal()) {
            int slotIndex = owner()->metaObject()->indexOfSlot(COLORPROPERTYCHANGEFUNC);
            if (slotIndex != -1)
                QMetaObject::connect(owner(), type()->signalOffset() + id, owner(), slotIndex, Qt::UniqueConnection);
        }
        return QQmlOpenMetaObject::propertyCreated(id, builder);
    }

    void propertyRead(int id) override {
        struct FakerItem : public QQuickItem {
            inline bool isComponentComplete() const {
                return QQuickItem::isComponentComplete();
            }
        };
        auto qmlObj = qobject_cast<QQuickItem*>(owner()->parent());
        if (qmlObj && static_cast<FakerItem*>(qmlObj)->isComponentComplete() && !value(id).value<QColor>().isValid()) {
            qWarning() << "ColorSelector: The" << name(id) << "is an invalid color on the" << qmlObj;
        }
        QQmlOpenMetaObject::propertyRead(id);
    }
};

DQuickControlColorSelector::DQuickControlColorSelector(QQuickItem *parent)
    : QObject(parent)
    , m_state(new PaletteState(this))
{
    ensureMetaObject();
    setupMetaPropertyPalettes(parent);
    findAndSetControlParent();
    updateFamilyForChildrenRecu(parent);
}

DQuickControlColorSelector::~DQuickControlColorSelector()
{

}

DQuickControlColorSelector *DQuickControlColorSelector::qmlAttachedProperties(QObject *object)
{
    if (auto window = qobject_cast<QQuickWindow*>(object)) {
        Q_ASSERT(window->contentItem());
        auto obj = qmlAttachedPropertiesObject<DQuickControlColorSelector>(window->contentItem());
        auto cs = qobject_cast<DQuickControlColorSelector*>(obj);
        Q_ASSERT(cs);
        return cs;
    }

    auto item = qobject_cast<QQuickItem*>(object);
    if (!item) {
        ThrowError(object, QStringLiteral("Cannot be used in non QQuickItem class."));
        return nullptr;
    }
    auto cs = new DQuickControlColorSelector(item);
    if (!cs->m_palettes.isEmpty()
            || specialObjectNameItems().contains(item->objectName())
            || _d_isControlItem(item)
            || _d_isWindowRootItem(item))
        return cs;

    delete cs;
    ThrowError(object, QStringLiteral("The Item[%1,ObjectName:%2] has not palettes included, Unable to create a change color selector.").
               arg(item->metaObject()->className(), item->objectName()));
    return nullptr;
}

void DQuickControlColorSelector::findAndSetControlParent()
{
    QQuickItem *parentItem = qobject_cast<QQuickItem*>(parent());
    Q_ASSERT(parentItem);
    for (const QMetaObject::Connection &conn : qAsConst(m_itemParentChangeConnections)) {
        disconnect(conn);
    }
    m_itemParentChangeConnections.clear();

    bool needUpdateControl = true;
    bool needUpdateColorFamily = !m_state->familyIsUserSet;
    do {
        if (needUpdateControl && _d_isControlItem(parentItem)) {
            needUpdateControl = false;
            setControl(parentItem);
        }

        if (needUpdateColorFamily && parentItem != parent()) {
            // try update color family
            auto cs = qobject_cast<DQuickControlColorSelector*>(qmlAttachedPropertiesObject<DQuickControlColorSelector>(parentItem, false));
            if (cs) {
                needUpdateColorFamily = false;
                setFamilyPropertyParent(cs);
            }
        }

        if (!needUpdateControl && !needUpdateColorFamily)
            break;

        QMetaObject::Connection conn = connect(parentItem, &QQuickItem::parentChanged,
                                               this, &DQuickControlColorSelector::findAndSetControlParent);
        if (conn)
            m_itemParentChangeConnections.append(conn);

        parentItem = parentItem->parentItem();
    } while (parentItem);

    if (needUpdateControl) {
        setControl(nullptr);
    }

    if (needUpdateColorFamily) {
        setFamilyPropertyParent(nullptr);
    }
}

QByteArray DQuickControlColorSelector::findPalettePropertyName(const DQuickControlPalette *palette) const
{
    if (!palette)
        return QByteArrayLiteral("");

    auto paletteDataFinder = [&](const ControlPaletteData &data) {
        return data.second == palette;
    };
    auto dataIter = std::find_if(m_palettes.cbegin(), m_palettes.cend(), paletteDataFinder);
    if (dataIter == m_palettes.cend()) {
        return QByteArrayLiteral("");
    }

    return dataIter->first;
}

void DQuickControlColorSelector::clearAndSetParentProperties()
{
    // Clear meta object properties that contained in it's old parent
    for (int i = 0; i < m_metaObject->count(); ++i) {
        auto name = m_metaObject->name(i);
        if (indexOfPalette(name) == -1
                && (!m_superColorSelector
                    || m_superColorSelector->indexOfPalette(name) == -1)) {
            m_metaObject->setValue(name, QVariant::Invalid);
        }
    }

    if (m_superColorSelector) {
        for (int i = 0; i < m_superColorSelector->m_metaObject->count(); ++i) {
            auto p = m_superColorSelector->m_metaObject->name(i);
            if (p.isEmpty())
                continue;
            updatePropertyFromName(p);
        }
    }
    Q_EMIT colorPropertiesChanged();
}

void DQuickControlColorSelector::setupMetaPropertyPalettes(QQuickItem *object)
{
    for (int i = 0; i < object->metaObject()->propertyCount(); ++i) {
        auto p = object->metaObject()->property(i);
        if (!p.name() || p.userType() != qMetaTypeId<DQuickControlPalette*>())
            continue;

        if (indexOfPalette(p.name()) != -1)
            ThrowError(this, QStringLiteral("ColorSelector: The Item %1(%2, name=\"%3\") already has a palette with name \"%4\", The latter one will replace the former one.")
                       .arg(object->metaObject()->className()).arg(QString::number(reinterpret_cast<quintptr>(object), 16).prepend("0x")).arg(object->objectName()).arg(p.name()));
        updatePaletteFromMetaProperty(p, object);
    }
}

QQuickItem *DQuickControlColorSelector::control() const
{
    return m_control;
}

void DQuickControlColorSelector::setControl(QQuickItem *newControl)
{
    if (m_control == newControl)
        return;

    // Clean for old
    if (m_control) {
        m_control->disconnect(this);
        if (auto w = m_control->window())
            w->disconnect(this);
    }

    m_control = newControl;

    if (m_control) {
        connect(m_control, SIGNAL(paletteChanged()), this, SLOT(updateControlTheme()));
        connect(m_control, SIGNAL(hoveredChanged()), this, SLOT(updateControlState()));
        if (m_control->metaObject()->indexOfSignal("pressedChanged()") != -1) {
            connect(m_control, SIGNAL(pressedChanged()), this, SLOT(updateControlState()));
        }
        connect(m_control, &QQuickItem::enabledChanged, this, &DQuickControlColorSelector::updateControlState);
        connect(m_control, &QQuickItem::windowChanged, this, &DQuickControlColorSelector::updateControlWindow);
        updateControlWindow();

        if (m_control != parent()) {
            auto csForControl = qobject_cast<DQuickControlColorSelector *>(
                        qmlAttachedPropertiesObject<DQuickControlColorSelector>(newControl, true));
            setSuperColorSelector(csForControl);
        }
    }

    Q_EMIT controlChanged();
}

void DQuickControlColorSelector::setSuperColorSelector(DQuickControlColorSelector *parent)
{
    if (m_superColorSelector == parent)
        return;
    Q_ASSERT(parent != this);

    if (m_superColorSelector) {
        m_superColorSelector->disconnect(this);
    }

    m_superColorSelector = parent;

    if (parent) {
        connect(parent, &DQuickControlColorSelector::palettesChanged, this, &DQuickControlColorSelector::updateAllColorProperties);
        connect(parent, &DQuickControlColorSelector::palettesChanged, this, &DQuickControlColorSelector::palettesChanged);
        connect(parent, &DQuickControlColorSelector::colorPropertyChanged,
                this, std::bind(&DQuickControlColorSelector::updatePropertyFromName, this, std::placeholders::_1, nullptr));
        connect(parent, &DQuickControlColorSelector::colorPropertiesChanged, this, &DQuickControlColorSelector::clearAndSetParentProperties);
        connect(parent, &DQuickControlColorSelector::destroyed, this, std::bind(
                    &DQuickControlColorSelector::setSuperColorSelector, this, nullptr));
    }

    clearAndSetParentProperties();
    updateAllColorProperties();

    Q_EMIT palettesChanged();
}

DGuiApplicationHelper::ColorType DQuickControlColorSelector::controlTheme() const
{
    return m_state->controlTheme;
}

DQuickControlPalette::ColorFamily DQuickControlColorSelector::family() const
{
    return m_state->family;
}

void DQuickControlColorSelector::setFamily(const DQuickControlPalette::ColorFamily &newFamily)
{
    m_state->familyIsUserSet = true;
    doSetFamily(newFamily);
}

void DQuickControlColorSelector::resetFamily()
{
    m_state->familyIsUserSet = false;
    doResetFamily();
}

DQMLGlobalObject::ControlState DQuickControlColorSelector::controlState() const
{
    return m_state->controlState;
}

void DQuickControlColorSelector::setControlTheme(DGuiApplicationHelper::ColorType theme)
{
    if (m_state->controlTheme == theme)
        return;

    m_state->controlTheme = theme;
    Q_EMIT controlThemeChanged();
    updateAllColorProperties();
}

void DQuickControlColorSelector::setControlState(DQMLGlobalObject::ControlState controlState)
{
    if (m_state->controlState == controlState)
        return;

    m_state->controlState = controlState;
    Q_EMIT controlStateChanged();
    updateAllColorProperties();
}

bool DQuickControlColorSelector::doSetFamily(DQuickControlPalette::ColorFamily newFamily)
{
    if (m_state->family == newFamily)
        return false;
    m_state->family = newFamily;
    Q_EMIT familyChanged();
    updateAllColorProperties();
    return true;
}

void DQuickControlColorSelector::doResetFamily()
{
    if (m_state->familyIsUserSet)
        return;
    const auto colorFamily = m_parentOfFamilyProperty ? m_parentOfFamilyProperty->family()
                                                      : DQuickControlPalette::CommonColor;
    doSetFamily(colorFamily);
}

void DQuickControlColorSelector::destroyPalette(DQuickControlPalette *palette)
{
    if (!palette)
        return;

    palette->disconnect(this);
    if (palette->parent() == this)
        palette->deleteLater();
}

bool DQuickControlColorSelector::hovered() const
{
    return m_state->hovered;
}

void DQuickControlColorSelector::setHovered(bool newHovered)
{
    m_state->hoveredValueValid = true;
    if (m_state->hovered == newHovered)
        return;
    m_state->hovered = newHovered;
    Q_EMIT hoveredChanged();
}

void DQuickControlColorSelector::resetHovered()
{
    setHovered(false);
    m_state->hoveredValueValid = false;
}

bool DQuickControlColorSelector::pressed() const
{
    return m_state->pressed;
}

void DQuickControlColorSelector::setPressed(bool newPressed)
{
    m_state->pressedValueValid = true;
    if (m_state->pressed == newPressed)
        return;
    m_state->pressed = newPressed;
    Q_EMIT pressedChanged();
}

void DQuickControlColorSelector::resetPressed()
{
    setPressed(false);
    m_state->pressedValueValid = false;
}

bool DQuickControlColorSelector::disabled() const
{
    return m_state->disabled;
}

void DQuickControlColorSelector::setDisabled(bool newDisabled)
{
    m_state->disabledValueValid = true;
    if (m_state->disabled == newDisabled)
        return;
    m_state->disabled = newDisabled;
    Q_EMIT disabledChanged();
}

void DQuickControlColorSelector::resetDisabled()
{
    setDisabled(false);
    m_state->disabledValueValid = false;
}

bool DQuickControlColorSelector::inactived() const
{
    return m_state->inactived;
}

void DQuickControlColorSelector::setInactived(bool newInactived)
{
    m_state->inactivedValueValid = true;
    if (m_state->inactived == newInactived)
        return;
    m_state->inactived = newInactived;
    Q_EMIT inactivedChanged();
}

void DQuickControlColorSelector::resetInactived()
{
    setInactived(false);
    m_state->inactivedValueValid = false;
}

int DQuickControlColorSelector::palette_count(QQmlListProperty<DQuickControlPalette> *property) {
    auto that = qobject_cast<DQuickControlColorSelector*>(property->object);
    return that->paletteCount();
}

DQuickControlPalette *DQuickControlColorSelector::palette_at(QQmlListProperty<DQuickControlPalette> *property, int index) {
    auto that = qobject_cast<DQuickControlColorSelector*>(property->object);
    return that->paletteAt(index);
}

void DQuickControlColorSelector::setFamilyPropertyParent(DQuickControlColorSelector *parent)
{
    if (m_parentOfFamilyProperty == parent)
        return;

    if (m_parentOfFamilyProperty) {
        disconnect(m_parentOfFamilyProperty.data(), &DQuickControlColorSelector::familyChanged,
                   this, &DQuickControlColorSelector::doResetFamily);
    }

    m_parentOfFamilyProperty = parent;

    if (m_parentOfFamilyProperty) {
        connect(m_parentOfFamilyProperty.data(), &DQuickControlColorSelector::familyChanged,
                this, &DQuickControlColorSelector::doResetFamily);
    }
    doResetFamily();
}

QQmlListProperty<DQuickControlPalette> DQuickControlColorSelector::palettes()
{
    return QQmlListProperty<DQuickControlPalette>(this, this, nullptr, palette_count,
                                                  palette_at, nullptr);
}

DQuickControlPalette *DQuickControlColorSelector::paletteAt(int index) const
{
    int parentPaletteCount = m_superColorSelector ? m_superColorSelector->paletteCount() : 0;
    if (index < parentPaletteCount)
        return m_superColorSelector->paletteAt(index);

    return m_palettes.at(index - parentPaletteCount).second;
}

int DQuickControlColorSelector::paletteCount() const
{
    return m_palettes.count() + (m_superColorSelector ? m_superColorSelector->paletteCount()
                                                      : 0);
}

QStringList DQuickControlColorSelector::specialObjectNameItems()
{
    // TODO(Chen Bin): To be determined
    return { QLatin1String("ColorSelectorMaster") };
}

static inline QColor getColor(const DQuickControlPalette *palette, int themeIndex, int familyIndex, int stateIndex) {
    QColor color = palette->colors.at(themeIndex + familyIndex + stateIndex);

    if (!color.isValid() && familyIndex > 0 && stateIndex > 0) {
        // fallback to normal state
        color = palette->colors.at(themeIndex + familyIndex);

        if (!color.isValid()) {
            // fallback to common family
            color = palette->colors.at(themeIndex + stateIndex);
        }
    }

    if (!color.isValid()) {
        // fallback to normal state and the common family and normal state
        color = palette->colors.at(themeIndex);
    }

    return color;
}

QColor DQuickControlColorSelector::getColorOf(const DQuickControlPalette *palette, const PaletteState *state)
{
    QColor targetColor;

    int themeIndex = DQuickControlPalette::Light;
    if (state->controlTheme == DGuiApplicationHelper::DarkType) {
        themeIndex = DQuickControlPalette::Dark;
    }

    const int familyIndex = state->family;

    int stateIndex = DQuickControlPalette::Normal;
    bool disabled = state->disabledValueValid ? state->disabled : (state->controlState == DQMLGlobalObject::DisabledState);
    if (disabled) {
        targetColor = palette->colors.at(themeIndex + DQuickControlPalette::Disabled);
        if (targetColor.isValid()) // Don't process the disabled's color, should direct uses it.
            return targetColor;
        // fallback to normal color
    } else if (state->pressedValueValid ? state->pressed : (state->controlState == DQMLGlobalObject::PressedState)) {
        stateIndex = DQuickControlPalette::Pressed;
    } else if (state->hoveredValueValid ? state->hovered : (state->controlState == DQMLGlobalObject::HoveredState)) {
        stateIndex = DQuickControlPalette::Hovered;
    }

    targetColor = getColor(palette, themeIndex, familyIndex, stateIndex);
    if (!targetColor.isValid() && state->controlTheme == DGuiApplicationHelper::DarkType) {
        // create the dark color from the light theme
        targetColor = getColor(palette, DQuickControlPalette::Light, familyIndex, stateIndex);
        // inverse the color to dark
        int r, g, b, a;
        targetColor.getRgb(&r, &g, &b, &a);
        targetColor = QColor(255 - r, 255 - g, 255 - b, a);
    }

    if (!targetColor.isValid() || !state->owner->m_control || disabled)
        return targetColor;

    if ((state->inactivedValueValid ? state->inactived : (state->owner->m_controlWindow && !state->owner->m_controlWindow->isActive()))
            && DGuiApplicationHelper::testAttribute(DGuiApplicationHelper::Attribute::UseInactiveColorGroup)) {
        const QPalette pa = qvariant_cast<QPalette>(state->owner->m_control->property("palette"));
        const QColor windowColor = pa.color(QPalette::Window);
        if (!windowColor.isValid())
            return targetColor;

        QColor inactive_mask_color = windowColor;

        if (state->controlTheme == DGuiApplicationHelper::DarkType) {
            inactive_mask_color.setAlphaF(0.6);
        } else {
            inactive_mask_color.setAlphaF(0.4);
        }

        targetColor = DGuiApplicationHelper::blendColor(targetColor, inactive_mask_color);
    }

    return targetColor;
}

QColor DQuickControlColorSelector::getColorOf(const QByteArray &propertyName, const PaletteState *state)
{
    // Ensure that only the last palette is used, other palettes with the same
    // name property are ignored.
    int paletteIndex = indexOfPalette(propertyName);
    if (paletteIndex != -1) {
        auto pal = this->m_palettes.at(paletteIndex).second;
        if (pal && pal->enabled()) {
            const QColor color = getColorOf(pal, state);
            return color;
        }
    }

    if (m_superColorSelector) {
        return m_superColorSelector->getColorOf(propertyName, state);
    }
    return QColor::Invalid;
}

void DQuickControlColorSelector::ensureMetaObject()
{
    if (m_metaObject)
        return;

    m_metaObject = new CustomMetaObject(this);
    m_metaObject->setCached(true);
}

int DQuickControlColorSelector::indexOfPalette(const QByteArray &name) const
{
    for (int i = 0; i < m_palettes.count(); ++i) {
        const auto &item = m_palettes.at(i);
        if (item.first == name)
            return i;
    }

    return -1;
}

void DQuickControlColorSelector::setPalette(const QByteArray &name, DQuickControlPalette *palette)
{
    int index = indexOfPalette(name);
    if (index < 0) {
        m_palettes << qMakePair(name, palette);
    } else {
        auto pal = m_palettes[index].second;
        if (pal == palette)
            return;
        destroyPalette(pal);
        m_palettes[index].second = palette;
    }

    if (palette) {
        connect(palette, &DQuickControlPalette::changed, this, &DQuickControlColorSelector::recvPaletteColorChanged, Qt::UniqueConnection);
        connect(palette, &DQuickControlPalette::enabledChanged, this, &DQuickControlColorSelector::recvPaletteColorChanged, Qt::UniqueConnection);
    }

    updatePropertyFromName(name, palette);
    Q_EMIT palettesChanged();
}

void DQuickControlColorSelector::updatePaletteFromMetaProperty(const QMetaProperty &mp, const QObject *obj)
{
    DQuickControlPalette *value = qvariant_cast<DQuickControlPalette*>(mp.read(obj));
    setPalette(mp.name(), value);

    if (mp.hasNotifySignal()) {
        int slotIndex = this->metaObject()->indexOfSlot(METAPROPERTYCHANGEFUNC);
        if (slotIndex != -1)
            QMetaObject::connect(obj, mp.notifySignalIndex(), this, slotIndex, Qt::UniqueConnection);
    }
}

void DQuickControlColorSelector::resolveMetaPropertyChanged()
{
    const QObject *obj = sender();
    Q_ASSERT(obj);
    QMetaProperty itemProperty = findMetaPropertyFromSignalIndex(obj, senderSignalIndex());
    if (!itemProperty.name() || itemProperty.userType() != qMetaTypeId<DQuickControlPalette*>())
        return;

    DQuickControlPalette *newPalette = qvariant_cast<DQuickControlPalette *>(itemProperty.read(obj));
    setPalette(itemProperty.name(), newPalette);
}

void DQuickControlColorSelector::notifyColorPropertyChanged()
{
    const QObject *obj = sender();
    Q_ASSERT(obj);
    QMetaProperty itemProperty = findMetaPropertyFromSignalIndex(obj, senderSignalIndex());
    const QByteArray &dynamicColorName = itemProperty.name();
    if (dynamicColorName.isEmpty())
        return;

    Q_EMIT colorPropertyChanged(dynamicColorName);
}

void DQuickControlColorSelector::updatePropertyFromName(const QByteArray &name, const DQuickControlPalette *palette)
{
    Q_ASSERT(!name.isEmpty());
    QColor color;
    // Always use the palette state in this class.
    if (palette && palette->enabled()) {
        color = getColorOf(palette, m_state.data());
    } else {
        color = getColorOf(name, m_state.data());
    }
    m_metaObject->setValue(name, color);
}

void DQuickControlColorSelector::updateFamilyForChildrenRecu(QQuickItem *parent)
{
    const auto childItems = parent->childItems();
    for (auto *child : childItems) {
        auto cs = qobject_cast<DQuickControlColorSelector*>(qmlAttachedPropertiesObject<DQuickControlColorSelector>(child, false));
        if (cs) {
            cs->setFamilyPropertyParent(this);
        } else {
            updateFamilyForChildrenRecu(child);
        }
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

void DQuickControlColorSelector::updateAllColorProperties()
{
    for (int i = 0; i < m_metaObject->count(); ++i) {
        auto p = m_metaObject->name(i);
        if (p.isEmpty())
            continue;
        updatePropertyFromName(p);
    }
}

void DQuickControlColorSelector::recvPaletteColorChanged()
{
    auto palette = qobject_cast<DQuickControlPalette*>(sender());
    Q_ASSERT(palette);
    QByteArray palName = findPalettePropertyName(palette);
    if (palName.isEmpty())
        return;
    updatePropertyFromName(palName, palette);
}

void DQuickControlColorSelector::updateControlWindow()
{
    if (m_controlWindow == m_control->window())
        return;
    if (m_controlWindow) {
        m_controlWindow->disconnect(this);
    }
    m_controlWindow = m_control->window();
    if (m_controlWindow) {
        connect(m_controlWindow, &QQuickWindow::activeChanged,
                this, &DQuickControlColorSelector::updateAllColorProperties);
        updateAllColorProperties();
    }
}

DQUICK_END_NAMESPACE
