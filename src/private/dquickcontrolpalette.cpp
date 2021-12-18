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

DQuickControlColor::DQuickControlColor()
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
};

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

DQuickControlColorSelector::DQuickControlColorSelector()
{

}

DQuickControlColorSelector::~DQuickControlColorSelector()
{

}

void DQuickControlColorSelector::palette_append(QQmlListProperty<DQuickControlPalette> *property, DQuickControlPalette *value) {
    if (!value)
        return;
    auto that = qobject_cast<DQuickControlColorSelector*>(property->object);
    if (!value->parent())
        value->setParent(that);
    that->m_palettes.append(value);
    that->ensureMetaObject();
    that->addPropertyForPalette(value);
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
    that->ensureMetaObject();
    for (auto palette : qAsConst(that->m_palettes))
        that->removePropertyForPalette(palette);
    that->m_palettes.clear();
    Q_EMIT that->palettesChanged();
}

QQmlListProperty<DQuickControlPalette> DQuickControlColorSelector::palettes()
{
    return QQmlListProperty<DQuickControlPalette>(this, this, palette_append, palette_count, palette_at, palette_clear);;
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
        connect(m_control, SIGNAL(paletteChanged()), this, SLOT(updateProperties()));
        connect(m_control, SIGNAL(hoveredChanged()), this, SLOT(updateProperties()));
        connect(m_control, SIGNAL(pressedChanged()), this, SLOT(updateProperties()));
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

QColor DQuickControlColorSelector::getColorOf(const DQuickControlPalette *palette)
{
    QColor targetColor;
    if (!m_control)
        return targetColor;

    const QPalette pa = qvariant_cast<QPalette>(m_control->property("palette"));
    const QColor windowColor = pa.color(QPalette::Window);

    if (!windowColor.isValid())
        return targetColor;

    int themeIndex = DQuickControlPalette::Light;
    const auto themeType = DGuiApplicationHelper::toColorType(windowColor);
    if (themeType == DGuiApplicationHelper::DarkType) {
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
    if (!m_control->isEnabled()) {
        targetColor = palette->colors.at(themeIndex + DQuickControlPalette::Disabled);
        if (!targetColor.isValid() && themeType == DGuiApplicationHelper::DarkType) {
            targetColor = palette->colors.at(DQuickControlPalette::Light + DQuickControlPalette::Disabled);
        }

        if (targetColor.isValid()) // Don't process the disabled's color, should direct uses it.
            return targetColor;
        // fallback to normal color
    } else if (m_control->property("pressed").toBool()) {
        stateIndex = DQuickControlPalette::Pressed;
    } else if (m_control->property("hovered").toBool()) {
        stateIndex = DQuickControlPalette::Hover;
    }

    targetColor = getColor(palette, themeIndex, stateIndex);
    if (!targetColor.isValid()) {
        // create the dark color from the light theme
        if (themeType == DGuiApplicationHelper::DarkType) {
            targetColor = getColor(palette, DQuickControlPalette::Light, stateIndex);
            // inverse the color to dark
            int r, g, b, a;
            targetColor.getRgb(&r, &g, &b, &a);
            targetColor = QColor(255 - r, 255 - g, 255 - b, a);
        }
    }

    if (!targetColor.isValid())
        return targetColor;

    QColor disable_mask_color = windowColor, inactive_mask_color = windowColor;

    if (themeType == DGuiApplicationHelper::DarkType) {
        disable_mask_color.setAlphaF(0.7);
        inactive_mask_color.setAlphaF(0.6);
    } else {
        disable_mask_color.setAlphaF(0.6);
        inactive_mask_color.setAlphaF(0.4);
    }

    if (!m_control->isEnabled()) {
        targetColor = DGuiApplicationHelper::blendColor(targetColor, disable_mask_color);
    } else if (m_control->window() && !m_control->window()->isActive()
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
        addPropertyForPalette(palette);
    }
}

void DQuickControlColorSelector::addPropertyForPalette(const DQuickControlPalette *palette)
{
    const auto id = palette->objectName();
    if (!id.isEmpty()) {
        const QColor color = getColorOf(palette);
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

void DQuickControlColorSelector::updateProperties()
{
    for (auto palette : qAsConst(m_palettes))
        addPropertyForPalette(palette);
}

void DQuickControlColorSelector::onPaletteChanged()
{
    auto palette = qobject_cast<DQuickControlPalette*>(sender());
    Q_ASSERT(palette && m_palettes.contains(palette));
    addPropertyForPalette(palette);
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
