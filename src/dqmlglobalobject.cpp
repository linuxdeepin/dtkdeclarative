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

#include "dqmlglobalobject.h"
#include "private/dqmlglobalobject_p.h"
#include "private/dquickcontrolpalette_p.h"
#include "private/dquickdciicon_p.h"
#include "private/dmessagemanager_p.h"

#include <DObjectPrivate>
#include <DObject>
#include <DGuiApplicationHelper>
#include <DFontManager>
#include <DSysInfo>
#include <QQuickItem>

#ifdef Q_OS_UNIX
#include <unistd.h>
#include <pwd.h>
#endif

DGUI_USE_NAMESPACE

DQUICK_BEGIN_NAMESPACE

DQMLGlobalObjectPrivate::DQMLGlobalObjectPrivate(DQMLGlobalObject *qq)
    : DTK_CORE_NAMESPACE::DObjectPrivate(qq)
{
}

void DQMLGlobalObjectPrivate::ensurePalette()
{
    if (paletteInit)
        return;

    paletteInit = true;
    updatePalettes();

    QObject::connect(DGuiApplicationHelper::instance(), SIGNAL(applicationPaletteChanged()), q_func(), SLOT(_q_onPaletteChanged()));
}

void DQMLGlobalObjectPrivate::updatePalettes()
{
    palette = DGuiApplicationHelper::instance()->applicationPalette();
    inactivePalette = palette;

    for (int i = 0; i < QPalette::NColorRoles; ++i) {
        QPalette::ColorRole role = static_cast<QPalette::ColorRole>(i);
        const QBrush &brush = palette.brush(QPalette::Inactive, role);
        inactivePalette.setBrush(QPalette::Active, role, brush);
    }
}

void DQMLGlobalObjectPrivate::_q_onPaletteChanged()
{
    updatePalettes();

    Q_EMIT q_func()->paletteChanged();
    Q_EMIT q_func()->inactivePaletteChanged();
}

void DQMLGlobalObjectPrivate::ensureWebsiteInfo()
{
    DCORE_USE_NAMESPACE;
    if (deepinWebsiteName.isEmpty() || deepinWebsiteLink.isEmpty()) {
        const auto &deepinWebsiteInfo = DSysInfo::distributionOrgWebsite(DSysInfo::Distribution);
        deepinWebsiteName = deepinWebsiteInfo.first;
        deepinWebsiteLink = deepinWebsiteInfo.second;
    }
}

DQMLGlobalObject::DQMLGlobalObject(QObject *parent)
    : QObject(parent)
    , DTK_CORE_NAMESPACE::DObject(*new DQMLGlobalObjectPrivate(this))
{
    auto pHelper = DWindowManagerHelper::instance();
    connect(pHelper, &DWindowManagerHelper::hasBlurWindowChanged, this, &DQMLGlobalObject::hasBlurWindowChanged);
    connect(pHelper, &DWindowManagerHelper::hasCompositeChanged, this, &DQMLGlobalObject::hasCompositeChanged);
    connect(pHelper, &DWindowManagerHelper::hasNoTitlebarChanged, this, &DQMLGlobalObject::hasNoTitlebarChanged);

    auto pAppHelper = DGuiApplicationHelper::instance();
    connect(pAppHelper, &DGuiApplicationHelper::themeTypeChanged, this, &DQMLGlobalObject::themeTypeChanged);
}

DQMLGlobalObject::~DQMLGlobalObject()
{
}

bool DQMLGlobalObject::hasBlurWindow() const
{
    return DWindowManagerHelper::instance()->hasBlurWindow();
}

bool DQMLGlobalObject::hasComposite() const
{
    return DWindowManagerHelper::instance()->hasComposite();
}

bool DQMLGlobalObject::hasNoTitlebar() const
{
    return DWindowManagerHelper::instance()->hasNoTitlebar();
}

bool DQMLGlobalObject::isSoftwareRender() const
{
    static bool isSoftware = QQuickWindow::sceneGraphBackend() == QLatin1String("software");
    return isSoftware;
}

QString DQMLGlobalObject::windowManagerNameString() const
{
    return DWindowManagerHelper::instance()->windowManagerNameString();
}

DGuiApplicationHelper::ColorType DQMLGlobalObject::themeType() const
{
    return DGuiApplicationHelper::instance()->themeType();
}

DWindowManagerHelper::WMName DQMLGlobalObject::windowManagerName() const
{
    return DWindowManagerHelper::instance()->windowManagerName();
}

DPlatformThemeProxy *DQMLGlobalObject::platformTheme() const
{
    D_DC(DQMLGlobalObject);

    if (!d->platformTheme) {
        d->platformTheme = new DPlatformThemeProxy(DGuiApplicationHelper::instance()->applicationTheme(), const_cast<DQMLGlobalObject *>(this));
    }

    return d->platformTheme;
}

DFontManager *DQMLGlobalObject::fontManager() const
{
    // qml中只能识别非const的指针属性
    return const_cast<DFontManager*>(DGuiApplicationHelper::instance()->fontManager());
}

QPalette DQMLGlobalObject::palette() const
{
    D_DC(DQMLGlobalObject);
    const_cast<DQMLGlobalObjectPrivate*>(d)->ensurePalette();
    return d->palette;
}

QPalette DQMLGlobalObject::inactivePalette() const
{
    D_DC(DQMLGlobalObject);
    const_cast<DQMLGlobalObjectPrivate*>(d)->ensurePalette();
    return d->inactivePalette;
}

QColor DQMLGlobalObject::adjustColor(const QColor &base, qint8 hueFloat, qint8 saturationFloat, qint8 lightnessFloat, qint8 redFloat, qint8 greenFloat, qint8 blueFloat, qint8 alphaFloat)
{
    return DGuiApplicationHelper::adjustColor(base, hueFloat, saturationFloat, lightnessFloat, redFloat, greenFloat, blueFloat, alphaFloat);
}

QColor DQMLGlobalObject::blendColor(const QColor &substrate, const QColor &superstratum)
{
    return DGuiApplicationHelper::blendColor(substrate, superstratum);
}

DGuiApplicationHelper::ColorType DQMLGlobalObject::toColorType(const QColor &color)
{
    return DGuiApplicationHelper::toColorType(color);
}

QColor DQMLGlobalObject::selectColor(const QColor &windowColor, const QColor &light, const QColor &dark)
{
    if (toColorType(windowColor) == DGuiApplicationHelper::DarkType) {
        return dark;
    } else {
        return light;
    }
}

QString DQMLGlobalObject::deepinWebsiteName() const
{
    D_DC(DQMLGlobalObject);

    const_cast<DQMLGlobalObjectPrivate*>(d)->ensureWebsiteInfo();

    return d->deepinWebsiteName;
}

QString DQMLGlobalObject::deepinWebsiteLink() const
{
    D_DC(DQMLGlobalObject);

    const_cast<DQMLGlobalObjectPrivate*>(d)->ensureWebsiteInfo();

    return d->deepinWebsiteLink;
}

QString DQMLGlobalObject::deepinDistributionOrgLogo() const
{
    DCORE_USE_NAMESPACE;

    D_DC(DQMLGlobalObject);
    if (d->deepinDistributionOrgLogo.isEmpty()) {
        const auto &logo = DSysInfo::distributionOrgLogo(DSysInfo::Distribution, DSysInfo::Light, ":/assets/images/deepin-logo.svg");
        const_cast<DQMLGlobalObjectPrivate*>(d)->deepinDistributionOrgLogo = logo;

    }
    return d->deepinDistributionOrgLogo;

}

QPoint DQMLGlobalObject::cursorPosition() const
{
    return QCursor::pos();
}

DQuickDciIcon DQMLGlobalObject::makeIcon(const QJSValue &qicon, const QJSValue &iconExtra)
{
    if (!qicon.isObject() || !iconExtra.isObject())
        return {};

    const QString &name = qicon.property("name").toString();
    int width = qicon.property("width").toInt();
    int height = qicon.property("height").toInt();
    const QColor &color = qicon.property("color").toVariant().value<QColor>();

    int type = iconExtra.property("type").toInt();
    int mode = iconExtra.property("mode").toInt();
    int theme = iconExtra.property("theme").toInt();

    DQuickDciIcon dciIcon;
    dciIcon.setName(name);
    dciIcon.setWidth(width);
    dciIcon.setHeight(height);
    dciIcon.setColor(color);
    dciIcon.setType(DQuickDciIconImage::Type(type));
    dciIcon.setMode(ControlState(mode));
    dciIcon.setTheme(DQuickDciIconImage::Theme(theme));

    return dciIcon;
}

bool DQMLGlobalObject::sendMessage(QQuickItem *target, const QString &content, const QString &iconName, int duration, const QString &msgId)
{
    Q_ASSERT(target);

    if (auto window = target->window())
        return sendMessage(window, content, iconName, duration, msgId);

    return false;
}

bool DQMLGlobalObject::sendMessage(QQuickWindow *target, const QString &content, const QString &iconName, int duration, const QString &msgId)
{
    if (auto manager = qobject_cast<MessageManager *>(qmlAttachedPropertiesObject<MessageManager>(target))) {
        return manager->sendMessage(content, iconName, duration, msgId);
    }
    return false;
}

bool DQMLGlobalObject::sendMessage(QQuickItem *target, QQmlComponent *delegate, const QVariant &message, int duration, const QString &msgId)
{
    Q_ASSERT(target);

    if (auto window = target->window())
        return sendMessage(window, delegate, message, duration, msgId);

    return false;
}

bool DQMLGlobalObject::sendMessage(QQuickWindow *target, QQmlComponent *delegate, const QVariant &message, int duration, const QString &msgId)
{
    if (auto manager = qobject_cast<MessageManager *>(qmlAttachedPropertiesObject<MessageManager>(target)))
        return manager->sendMessage(delegate, message, duration, msgId);

    return false;
}

void DQMLGlobalObject::closeMessage(FloatingMessageContainer *message)
{
    Q_ASSERT(message);

    message->close();
}

void DQMLGlobalObject::closeMessage(QQuickItem *target, const QString &msgId)
{
    Q_ASSERT(target);

    if (auto window = target->window())
        closeMessage(window, msgId);
}

void DQMLGlobalObject::closeMessage(QQuickWindow *target, const QString &msgId)
{
    if (msgId.isEmpty())
        return;

    if (auto manager = qobject_cast<MessageManager *>(qmlAttachedPropertiesObject<MessageManager>(target))) {
        manager->close(msgId);
    }
}

DQUICK_END_NAMESPACE

#include "moc_dqmlglobalobject.cpp"
