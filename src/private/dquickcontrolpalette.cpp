// SPDX-FileCopyrightText: 2020 - 2026 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include "dquickcontrolpalette_p.h"
#include "dquickglobal_p.h"

#include <DGuiApplicationHelper>

#include <QPalette>
#include <QQuickItem>
#include <QQuickWindow>
#include <QDebug>
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
#include <private/qquickpalette_p.h>
#endif

#include <private/qqmlopenmetaobject_p.h>
#include <private/qqmlglobal_p.h>
#include <private/qmetaobjectbuilder_p.h>
#include <private/qcoreapplication_p.h>

DGUI_USE_NAMESPACE
DQUICK_BEGIN_NAMESPACE

#define METAPROPERTYCHANGEFUNC "resolveMetaPropertyChanged()"
#define COLORPROPERTYCHANGEFUNC "notifyColorPropertyChanged()"

static inline bool _d_isControlItem(QQuickItem *item) {
#define _D_FOR_EACH_SIMILAR_CONTROL_ITEM(F) \
    F(QQuickControl), \
    F(QQuickTextField)

#if defined(QT_NAMESPACE)
#define SIMILAR_CONTROL_ITEM_NAMESPACE_STR1(NAME, Item) #NAME "::" #Item
#define SIMILAR_CONTROL_ITEM_NAMESPACE_STR(R, Item) SIMILAR_CONTROL_ITEM_NAMESPACE_STR1(R, Item)
#define _D_SIMILAR_CONTROL_ITEM(Item) SIMILAR_CONTROL_ITEM_NAMESPACE_STR(QT_NAMESPACE, Item)
#else
#define _D_SIMILAR_CONTROL_ITEM(Item) #Item
#endif

    static QStringList controlItems {
        _D_FOR_EACH_SIMILAR_CONTROL_ITEM(_D_SIMILAR_CONTROL_ITEM)
    };

    auto iter = std::find_if(controlItems.cbegin(), controlItems.cend(), [item] (const QString &name) {
        return item->inherits(name.toLatin1());
    });

    return iter != controlItems.cend();
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

// it's maybe an bug for qt, to use qvariant_cast to get Control's palette.
// `qvariant_cast` gets application's palette, and `toQPalette` gets root window's
// palette.
static inline QPalette _d_getControlPalette(QQuickItem *item) {
    if (!item)
        return QPalette();

    const QVariant &palette = item->property("palette");
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    const QQuickPalette *pa = palette.value<QQuickPalette *>();
    if (!pa)
        return QPalette();
    return pa->toQPalette();
#else
    return qvariant_cast<QPalette>(palette);
#endif
}

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
// Build a palette used only for DTK color resolution: theme detection (which needs
// the Window role) and typed-color resolving (which needs Highlight /
// HighlightedText, see DColor::toColor).
//
// It must NOT be obtained via QQuickPalette::toQPalette(). That function reads
// back every palette role, including the ones DTK itself overrides from QML
// (e.g. `palette.windowText: D.ColorSelector.textColor` on Button / ToolButton /
// MenuItem / ...). Reading such a role re-triggers its QML binding, which then
// resolves the typed color through this very code path and reads the role again
// -- a self-referential cycle that Qt6 reports as
// "Binding loop detected for property windowText".
//
// Resolution only ever needs roles that DTK does not override from QML, so we copy
// just those -- one role at a time, which never touches the overridden roles --
// and fall back to the DTK standard palette of the current control theme for the
// rest. Each role is taken from the Active group, matching the previous
// `toQPalette().color(role)` behaviour (toQPalette() yields a palette whose
// currentColorGroup stays Active, regardless of the control's state).
static QPalette _d_getControlPaletteForResolve(QQuickItem *item, DGuiApplicationHelper::ColorType theme)
{
    QPalette palette = DGuiApplicationHelper::standardPalette(theme);
    if (item) {
        if (const QQuickPalette *pa = item->property("palette").value<QQuickPalette *>()) {
            if (QQuickColorGroup *active = pa->active()) {
                palette.setColor(QPalette::Window, active->window());
                palette.setColor(QPalette::Highlight, active->highlight());
                palette.setColor(QPalette::HighlightedText, active->highlightedText());
            }
        }
    }
    return palette;
}

// Tell whether the control's palette actually has a distinct Inactive group, i.e.
// whether it has a real inactive state. Used by getColorOf() to decide whether to
// blend the inactive mask color.
//
// Like _d_getControlPaletteForResolve(), this must NOT go through
// QQuickPalette::toQPalette(): that reads back every role -- including
// `windowText`, which DTK overrides from QML (`palette.windowText:
// D.ColorSelector.*`) -- and would re-trigger that QML binding while it is still
// on the evaluation stack (getColorOf() runs inside `palette.windowText`'s
// binding), producing the same "Binding loop detected for windowText" as the
// typed-color path. This is the third windowText read point on the getColorOf()
// call path and must be broken together with the other two.
//
// We copy every role EXCEPT WindowText, one role at a time, from the control's
// resolved active()/inactive() groups into a fresh QPalette and compare the two
// groups with isEqual(). WindowText is left at its default value, which is
// identical in both groups of a default-constructed QPalette, so it never
// affects the result. Excluding WindowText is not only required to break the
// loop but also semantically correct here: DTK binds `palette.windowText` to a
// D.ColorSelector value, so any active/inactive difference in windowText is an
// artifact of that binding rather than a real inactive state of the control,
// and must not flip the inactive-blend gate (otherwise the inactive text would
// always be blended and look disabled).
//
// A DTK standard palette is deliberately NOT used as the base: its Inactive
// group is already blended (generatePaletteColor_helper), which would make
// Inactive != Active unconditionally and defeat the "no real inactive state"
// gate.
static bool _d_controlPaletteHasInactiveState(QQuickItem *item)
{
    if (!item)
        return false;
    const QQuickPalette *pa = item->property("palette").value<QQuickPalette *>();
    if (!pa)
        return false;
    QQuickColorGroup *activeGroup = pa->active();
    QQuickColorGroup *inactiveGroup = pa->inactive();
    if (!activeGroup || !inactiveGroup)
        return false;

    // QQuickColorGroup::color(ColorRole) is private, so read each role through its
    // public per-role accessor.
    auto colorOf = [](QQuickColorGroup *g, QPalette::ColorRole r) -> QColor {
        switch (r) {
        case QPalette::WindowText:        return g->windowText();
        case QPalette::Button:           return g->button();
        case QPalette::Light:            return g->light();
        case QPalette::Midlight:         return g->midlight();
        case QPalette::Dark:             return g->dark();
        case QPalette::Mid:              return g->mid();
        case QPalette::Text:             return g->text();
        case QPalette::BrightText:       return g->brightText();
        case QPalette::ButtonText:       return g->buttonText();
        case QPalette::Base:             return g->base();
        case QPalette::Window:           return g->window();
        case QPalette::Shadow:           return g->shadow();
        case QPalette::Highlight:        return g->highlight();
        case QPalette::HighlightedText:  return g->highlightedText();
        case QPalette::Link:             return g->link();
        case QPalette::LinkVisited:      return g->linkVisited();
        case QPalette::AlternateBase:   return g->alternateBase();
        case QPalette::NoRole:           return QColor(); // no public accessor; equal by default in both groups
        case QPalette::ToolTipBase:      return g->toolTipBase();
        case QPalette::ToolTipText:      return g->toolTipText();
        case QPalette::PlaceholderText:  return g->placeholderText();
        case QPalette::Accent:          return g->accent();
        case QPalette::NColorRoles:      break;
        }
        return QColor();
    };

    QPalette compare;
    for (int role = QPalette::WindowText; role < QPalette::NColorRoles; ++role) {
        if (role == QPalette::WindowText || role == QPalette::NoRole)
            continue; // windowText: DTK overrides it from QML (binding artifact, not a real inactive state).
                     // NoRole: no public QQuickColorGroup accessor; left at default (equal in both groups).
        const auto cr = static_cast<QPalette::ColorRole>(role);
        compare.setColor(QPalette::Active, cr, colorOf(activeGroup, cr));
        compare.setColor(QPalette::Inactive, cr, colorOf(inactiveGroup, cr));
    }
    return !compare.isEqual(QPalette::Inactive, QPalette::Active);
}
#endif

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

DQuickControlColor::DQuickControlColor(const DColor &color)
    : data(new DColor(color))
    , isSingleColor(true)
{

}

DQuickControlColor::DQuickControlColor(DColor *colors)
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

const DColor &DQuickControlColor::common() const
{
    Q_ASSERT(data && !isSingleColor);
    return data[DQuickControlPalette::CommonColor];
}

void DQuickControlColor::setCommon(const DColor &newCommon)
{
    Q_ASSERT(data && !isSingleColor);
    if (common() == newCommon)
        return;
    changed =  true;
    data[DQuickControlPalette::CommonColor] = newCommon;
}

const DColor &DQuickControlColor::crystal() const
{
    Q_ASSERT(data && !isSingleColor);
    return data[DQuickControlPalette::CrystalColor];
}

void DQuickControlColor::setCrystal(const DColor &newCrystal)
{
    Q_ASSERT(data && !isSingleColor);
    if (crystal() == newCrystal)
        return;
    changed = true;
    data[DQuickControlPalette::CrystalColor] = newCrystal;
}

DQuickControlPalette::DQuickControlPalette(QObject *parent)
    : QObject(parent)
    , colors(ThemeTypeCount * ColorTypeCount * ColorFamilyCount, DColor())
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
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
        : QQmlOpenMetaObject(obj, new QQmlOpenMetaObjectType(obj->metaObject(), qmlEngine(obj->parent())))
#else
        : QQmlOpenMetaObject(obj, new QQmlOpenMetaObjectType(obj->metaObject()))
#endif
    {

    }

    ~CustomMetaObject() override
    {
        if (auto item = type()) {
            item->release();
        }
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
        if (_c == QMetaObject::ResetProperty && _id >= propertyOffset()) {
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
                QMetaObject::connect(object(), type()->signalOffset() + id, owner(), slotIndex, Qt::UniqueConnection);
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
            const auto pname = name(id);
            int palIndex  = owner()->indexOfPalette(pname);
            if (palIndex >= 0 && owner()->m_palettes.at(palIndex).second)
                qWarning() << "ColorSelector: The" << pname << "is an invalid color on the" << qmlObj;
        }
        QQmlOpenMetaObject::propertyRead(id);
    }

    // Because set true for QQmlOpenMetaObject::setCached, the DQuickControlColorSelector's
    // QQmlPropertyCache in QQmlData is from the QQmlOpenMetaObject. The QQmlData::propertyCache
    // is released in QQmlOpenMetaObject::createProperty, If we not reset it to owner()->m_propertyCache,
    // It's will reset by QQmlMetaTypeData::propertyCache's caller(qQmlPropertyCacheProperty), in Qt5,
    // the QQmlMetaTypeData will store the QQmlOpenMetaObject to QQmlMetaTypeData::propertyCaches for
    // a DynamicMetaObject(QQmlOpenMetaObject), and not update QQmlData::propertyCache when create new
    // property in QQmlOpenMetaObject, this is a bug, so we must ensure the QQmlData::propertyCache awlays
    // from QQmlOpenMetaObject.
#if QT_VERSION <= QT_VERSION_CHECK(6, 2, 4)
    int createProperty(const char *name, const char *data) override {
        QQmlData *qmldata = QQmlData::get(owner(), false);
        auto cache = qmldata ? qmldata->propertyCache : nullptr;
        Q_ASSERT(!cache || cache == owner()->m_propertyCache);

        int ret = QQmlOpenMetaObject::createProperty(name, data);
        if (qmldata && qmldata->propertyCache != cache) {
            Q_ASSERT(!qmldata->propertyCache); // It's released in QQmlOpenMetaObject::createProperty
            // Ensure QQmlOpenMetaObject::setCached always take effect
            qmldata->propertyCache = owner()->m_propertyCache;
            qmldata->propertyCache->addref();
        }

        return ret;
    }
#endif
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
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    for (const QMetaObject::Connection &conn : qAsConst(m_itemParentChangeConnections)) {
#else
    for (const QMetaObject::Connection &conn : std::as_const(m_itemParentChangeConnections)) {
#endif
        disconnect(conn);
    }
    m_itemParentChangeConnections.clear();

    bool needUpdateControl = true;
    bool needUpdateColorFamily = !m_state->familyIsUserSet;
    do {
        if (needUpdateControl && (_d_isControlItem(parentItem) || specialObjectNameItems().contains(parentItem->objectName()))) {
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

void DQuickControlColorSelector::clearAndInheritParentProperties()
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
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
        auto palette = m_control->property("palette").value<QQuickPalette*>();
        connect(palette, &QQuickPalette::changed, this, &DQuickControlColorSelector::updateControlTheme);
#endif
        connect(m_control, &QObject::destroyed, this, [this] {
            m_controlWindow = nullptr;
            m_control = nullptr;
        });
        if (m_control->metaObject()->indexOfSignal("paletteChanged()") != -1) {
            connect(m_control, SIGNAL(paletteChanged()), this, SLOT(updateControlTheme()));
        }
        if (m_control->metaObject()->indexOfSignal("hoveredChanged()") != -1) {
            connect(m_control, SIGNAL(hoveredChanged()), this, SLOT(updateControlState()));
        }
        if (m_control->metaObject()->indexOfSignal("pressedChanged()") != -1) {
            connect(m_control, SIGNAL(pressedChanged()), this, SLOT(updateControlState()));
        }
        connect(m_control, &QQuickItem::enabledChanged, this, &DQuickControlColorSelector::updateControlState);
        connect(m_control, &QQuickItem::visibleChanged, this, &DQuickControlColorSelector::updateControlState);
        connect(m_control, &QQuickItem::windowChanged, this, &DQuickControlColorSelector::updateControlWindow);
        updateControlWindow();
        updateControlTheme();
        updateControlState();

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
        connect(parent, &DQuickControlColorSelector::colorPropertyChanged,
                this, std::bind(&DQuickControlColorSelector::updatePropertyFromName, this, std::placeholders::_1, nullptr));
        connect(parent, &DQuickControlColorSelector::colorPropertiesChanged, this, &DQuickControlColorSelector::clearAndInheritParentProperties);
        connect(parent, &DQuickControlColorSelector::destroyed, this, std::bind(
                    &DQuickControlColorSelector::setSuperColorSelector, this, nullptr));
        connect(parent, &DQuickControlColorSelector::hoveredChanged, this, &DQuickControlColorSelector::updateControlState);
        connect(parent, &DQuickControlColorSelector::pressedChanged, this, &DQuickControlColorSelector::updateControlState);
        connect(parent, &DQuickControlColorSelector::disabledChanged, this, &DQuickControlColorSelector::updateControlState);
        connect(parent, &DQuickControlColorSelector::inactivedChanged, this, &DQuickControlColorSelector::updateControlState);
    }

    clearAndInheritParentProperties();
    if (!updateControlState())
        updateAllColorProperties();
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

bool DQuickControlColorSelector::setControlTheme(DGuiApplicationHelper::ColorType theme)
{
    if (m_state->controlTheme == theme)
        return false;

    m_state->controlTheme = theme;
    Q_EMIT controlThemeChanged();
    updateAllColorProperties();
    return true;
}

bool DQuickControlColorSelector::setControlState(DQMLGlobalObject::ControlState controlState)
{
    if (m_state->controlState == controlState)
        return false;

    m_state->controlState = controlState;
    Q_EMIT controlStateChanged();
    updateAllColorProperties();
    return true;
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

void DQuickControlColorSelector::tryDestroyPalette(DQuickControlPalette *palette)
{
    if (!palette)
        return;

    // This palette is used by the multi properties, we must ensure destroy it in last ref is freed.
    if (!findPalettePropertyName(palette).isEmpty()) {
        return;
    }

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
    doSetHovered(newHovered, true);
}

void DQuickControlColorSelector::resetHovered()
{
    doSetHovered(false, false);
}

bool DQuickControlColorSelector::pressed() const
{
    return m_state->pressed;
}

void DQuickControlColorSelector::setPressed(bool newPressed)
{
    doSetPressed(newPressed, true);
}

void DQuickControlColorSelector::resetPressed()
{
    doSetPressed(false, false);
}

bool DQuickControlColorSelector::disabled() const
{
    return m_state->disabled;
}

void DQuickControlColorSelector::setDisabled(bool newDisabled)
{
    doSetDisabled(newDisabled, true);
}

void DQuickControlColorSelector::resetDisabled()
{
    doSetDisabled(false, false);
}

bool DQuickControlColorSelector::inactived() const
{
    return m_state->inactived;
}

void DQuickControlColorSelector::setInactived(bool newInactived)
{
    doSetInactived(newInactived, true);
}

void DQuickControlColorSelector::resetInactived()
{
    doSetInactived(false, false);
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

QStringList DQuickControlColorSelector::specialObjectNameItems()
{
    // TODO(Chen Bin): To be determined
    return { QLatin1String("ColorSelectorMaster") };
}

bool DQuickControlColorSelector::doGetHoveredRecu(bool *value) const
{
    if (m_state->hoveredValueValid) {
        *value = m_state->hovered;
        return true;
    }

    return m_superColorSelector ? m_superColorSelector->doGetHoveredRecu(value) : false;
}

bool DQuickControlColorSelector::doGetPressedRecu(bool *value) const
{
    if (m_state->pressedValueValid) {
        *value = m_state->pressed;
        return true;
    }

    return m_superColorSelector ? m_superColorSelector->doGetPressedRecu(value) : false;
}

bool DQuickControlColorSelector::doGetDisabledRecu(bool *value) const
{
    if (m_state->disabledValueValid) {
        *value = m_state->disabled;
        return true;
    }

    return m_superColorSelector ? m_superColorSelector->doGetDisabledRecu(value) : false;
}

bool DQuickControlColorSelector::doGetInactivedRecu(bool *value) const
{
    if (m_state->inactivedValueValid) {
        *value = m_state->inactived;
        return true;
    }

    return m_superColorSelector ? m_superColorSelector->doGetInactivedRecu(value) : false;
}

void DQuickControlColorSelector::doSetHovered(bool newHovered, bool isUserSet)
{
    if (newHovered == m_state->hovered && isUserSet == m_state->hoveredValueValid)
        return;
    m_state->hoveredValueValid = isUserSet;
    m_state->hovered = newHovered;
    Q_EMIT hoveredChanged();
    updateControlState();
}

void DQuickControlColorSelector::doSetPressed(bool newPressed, bool isUserSet)
{
    if (newPressed == m_state->pressed && isUserSet == m_state->pressedValueValid)
        return;
    m_state->pressedValueValid = isUserSet;
    m_state->pressed = newPressed;
    Q_EMIT pressedChanged();
    updateControlState();
}

void DQuickControlColorSelector::doSetDisabled(bool newDisabled, bool isUserSet)
{
    if (newDisabled == m_state->disabled && isUserSet == m_state->disabledValueValid)
        return;
    m_state->disabledValueValid = isUserSet;
    m_state->disabled = newDisabled;
    Q_EMIT disabledChanged();
    updateControlState();
}

void DQuickControlColorSelector::doSetInactived(bool newInactived, bool isUserSet)
{
    if (newInactived == m_state->inactived && isUserSet == m_state->inactivedValueValid)
        return;
    m_state->inactivedValueValid = isUserSet;
    m_state->inactived = newInactived;
    Q_EMIT inactivedChanged();
    updateControlState();
}

static inline DColor getColor(const DQuickControlPalette *palette, int themeIndex, int familyIndex, int stateIndex) {
    DColor color = palette->colors.at(themeIndex + familyIndex + stateIndex);

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

QColor DQuickControlColorSelector::getColorOf(const DQuickControlPalette *palette, const PaletteState *state) const
{
    DColor targetColor;

    int themeIndex = DQuickControlPalette::Light;
    if (state->controlTheme == DGuiApplicationHelper::DarkType) {
        themeIndex = DQuickControlPalette::Dark;
    }

    const int familyIndex = state->family;

    int stateIndex = DQuickControlPalette::Normal;
    const bool disabled = state->controlState == DQMLGlobalObject::DisabledState;
    bool shouldInverseColor = false;
    do {
        if (disabled) {
            targetColor = palette->colors.at(themeIndex + familyIndex + DQuickControlPalette::Disabled);
            if (targetColor.isValid()) // Don't process the disabled's color, should direct uses it.
                break;
            // fallback to normal color
        } else if (state->controlState == DQMLGlobalObject::PressedState) {
            stateIndex = DQuickControlPalette::Pressed;
        } else if (state->controlState == DQMLGlobalObject::HoveredState) {
            stateIndex = DQuickControlPalette::Hovered;
        }

        targetColor = getColor(palette, themeIndex, familyIndex, stateIndex);
        if (!targetColor.isValid() && state->controlTheme == DGuiApplicationHelper::DarkType) {
            // create the dark color from the light theme
            targetColor = getColor(palette, DQuickControlPalette::Light, familyIndex, stateIndex);
            // inverse the color to dark
            if (targetColor.type() != DColor::Highlight && targetColor.type() != DColor::HighlightedText) {
                shouldInverseColor = true;
            }
        }
    } while (false);

    QColor colorValue;
    if (targetColor.isTypedColor()) {
        if (m_control) {
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
            // Resolve typed colors (Highlight / HighlightedText) against a palette
            // that only carries the non-DTK-overridden roles, so we never read back
            // `windowText` (which DTK binds to this very selector) and retrigger its
            // QML binding -- the root cause of the "Binding loop detected for
            // windowText" warning. Qt5 keeps the original path (no QQuickPalette,
            // no loop).
            colorValue = targetColor.toColor(_d_getControlPaletteForResolve(m_control, state->controlTheme));
#else
            colorValue = targetColor.toColor(_d_getControlPalette(m_control));
#endif
        }
    } else {
        colorValue = targetColor.color();
    }

    if (shouldInverseColor) {
        int r, g, b, a;
        colorValue.getRgb(&r, &g, &b, &a);
        colorValue = QColor(255 - r, 255 - g, 255 - b, a);
    }

    // items with inactive state should use inactive color, it likes dtkgui's generatePaletteColor_helper.
    static const auto useInactiveColor = DGuiApplicationHelper::testAttribute(DGuiApplicationHelper::UseInactiveColorGroup);
    bool shouldBlendInactive = useInactiveColor && state->controlState == DQMLGlobalObject::InactiveState;
    if (shouldBlendInactive) {
        if (m_control) {
            // If the control's inactive palette is same as active palette, it means the control does not have a real
            // inactive state, we should not blend the color with inactive mask color, otherwise it will cause the
            // color looks like disabled and hard to recognize.
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
            // Compare the control's Active/Inactive palette groups without going through
            // QQuickPalette::toQPalette() (the third windowText read point on this
            // getColorOf() call path -- it would read back the DTK-overwritten
            // `windowText` and re-trigger its QML binding, i.e. the same binding loop as
            // the typed-color path). The comparison excludes `windowText`: it is the
            // only role DTK overrides from QML, so any active/inactive difference there
            // is a binding artifact rather than a real inactive state and must not flip
            // this gate. See _d_controlPaletteHasInactiveState() for the full rationale.
            shouldBlendInactive = _d_controlPaletteHasInactiveState(m_control);
#else
            const auto qpalette = _d_getControlPalette(m_control);
            shouldBlendInactive = !qpalette.isEqual(QPalette::Inactive, QPalette::Active);
#endif
        }
    }
    if (shouldBlendInactive) {
        const auto &palette = DGuiApplicationHelper::standardPalette(state->controlTheme);
        const auto &windowColor = palette.color(QPalette::Window);

        QColor inactiveMaskColor = windowColor;
        inactiveMaskColor.setAlphaF(state->controlTheme == DGuiApplicationHelper::DarkType ? 0.6 : 0.4);
        colorValue = DGuiApplicationHelper::blendColor(colorValue, inactiveMaskColor);
    }

    return colorValue;
}

QColor DQuickControlColorSelector::getColorOf(const QByteArray &propertyName, const PaletteState *state) const
{
    // Ensure that only the last palette is used, other palettes with the same
    // name property are ignored.
    int paletteIndex = indexOfPalette(propertyName);
    if (paletteIndex != -1) {
        auto pal = this->m_palettes.at(paletteIndex).second;
        if (pal && pal->enabled()) {
            return getColorOf(pal, state);
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
    // TODO setCached will cause builtin property is undefined firstly in qml.
#if QT_VERSION < QT_VERSION_CHECK(6, 8, 0)
    // Must true, see CustomMetaObject::createProperty
    m_metaObject->setCached(true);
    QQmlData *qmldata = QQmlData::get(this);
    Q_ASSERT(qmldata);
    // the cache object is from QQmlOpenMetaObjectTypePrivate
    m_propertyCache = qmldata->propertyCache;
#endif
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
        auto oldPal = m_palettes[index].second;
        if (oldPal == palette)
            return;
        m_palettes[index].second = palette;
        tryDestroyPalette(oldPal);
    }

    if (palette) {
        connect(palette, &DQuickControlPalette::changed, this, &DQuickControlColorSelector::recvPaletteColorChanged, Qt::UniqueConnection);
        connect(palette, &DQuickControlPalette::enabledChanged, this, &DQuickControlColorSelector::recvPaletteColorChanged, Qt::UniqueConnection);
        // Destoryed in QML context
        connect(palette, &DQuickControlPalette::destroyed, this, &DQuickControlColorSelector::onPaletteDestroyed, Qt::UniqueConnection);
    }

    updatePropertyFromName(name, palette);
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
    if (QCoreApplication::closingDown())
        return;
    auto appriv = dynamic_cast<QCoreApplicationPrivate *>(QObjectPrivate::get(qApp));
    if (!appriv || appriv->aboutToQuitEmitted)
        return;
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

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    // Detect the control theme from its Window color without going through
    // QQuickPalette::toQPalette(), which would read back the DTK-overwritten
    // `windowText` and retrigger its QML binding (binding loop). The Window role
    // is never overridden by DTK, so reading it alone is both correct and safe.
    const QColor windowColor = _d_getControlPaletteForResolve(m_control, m_state->controlTheme).color(QPalette::Window);
#else
    const QPalette pa = _d_getControlPalette(m_control);
    const QColor windowColor = pa.color(QPalette::Window);
#endif

    if (!windowColor.isValid()) {
        // When the palette changed, should update the properties if it's DColor type is variant color.
        updateAllColorProperties();
        return;
    }

    const auto themeType = DGuiApplicationHelper::toColorType(windowColor);

    if (!setControlTheme(themeType)) {
        // When the palette changed, should update the properties if it's DColor type is variant color.
        updateAllColorProperties();
    }
}

bool DQuickControlColorSelector::updateControlState()
{
    DQMLGlobalObject::ControlState state = DQMLGlobalObject::NormalState;

    bool value = false;
    if (doGetDisabledRecu(&value) ? value : m_control && !m_control->isEnabled()) {
        state = DQMLGlobalObject::DisabledState;
    } else if (doGetPressedRecu(&value) ? value : m_control && m_control->property("pressed").toBool()) {
        state = DQMLGlobalObject::PressedState;
    } else if (doGetHoveredRecu(&value) ? value : m_control && m_control->property("hovered").toBool()) {
        state = DQMLGlobalObject::HoveredState;
    } else if (doGetInactivedRecu(&value) ? value : m_controlWindow && !m_controlWindow->isActive()) {
        state = DQMLGlobalObject::InactiveState;
    }

    return setControlState(state);
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
    // Maybe the multiple properties is use a same palette.
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    for (const auto &i : std::as_const(m_palettes)) {
#else
    for (const auto &i : qAsConst(m_palettes)) {
#endif
        if (i.second != palette)
            continue;
        updatePropertyFromName(i.first, palette);
    }
}

void DQuickControlColorSelector::onPaletteDestroyed()
{
    auto palette = sender();
    Q_ASSERT(palette);
    // Maybe the multiple properties is use a same palette.
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    for (const auto &i : std::as_const(m_palettes)) {
#else
    for (const auto &i : qAsConst(m_palettes)) {
#endif
        if (i.second != palette)
            continue;
        setPalette(i.first, nullptr);
    }
}

void DQuickControlColorSelector::updateControlWindow()
{
    QQuickWindow *window = m_control ? m_control->window() : nullptr;
    if (m_controlWindow == window)
        return;
    if (m_controlWindow) {
        m_controlWindow->disconnect(this);
    }
    m_controlWindow = window;
    if (m_controlWindow) {
        connect(m_controlWindow, &QQuickWindow::activeChanged,
                this, &DQuickControlColorSelector::updateControlState);
        updateAllColorProperties();
    }
}

DQUICK_END_NAMESPACE
