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
#include "dquickdciiconimage_p.h"
#include "dquickiconimage_p_p.h"

#include <dobject_p.h>
#include <DIconTheme>
#include <DGuiApplicationHelper>
#include <DPlatformTheme>

#include <QUrlQuery>

DQUICK_BEGIN_NAMESPACE
DGUI_USE_NAMESPACE

static QString appIconThemeName()
{
    return DGuiApplicationHelper::instance()->applicationTheme()->iconThemeName();
}

class DQuickDciIconImagePrivate : public DQuickIconImagePrivate
{
    Q_DECLARE_PUBLIC(DQuickDciIconImage)

public:
    void maybeUpdateUrl();
    QUrlQuery getUrlQuery();

    QString name;
    DQMLGlobalObject::ControlState mode = DQMLGlobalObject::NormalState;
    DQuickDciIconImage::Theme theme = DQuickDciIconImage::Light;
    DDciIconPalette palette;
};

void DQuickDciIconImagePrivate::maybeUpdateUrl()
{
    Q_Q(DQuickDciIconImage);
    if (name.isEmpty()) {
        q->setSource(QUrl());
        return;
    }

    QUrl url;
    url.setScheme(QLatin1String("image"));
    url.setHost(QLatin1String("dtk.dci.icon"));
    url.setQuery(getUrlQuery());
    q->setSource(url);
}

QUrlQuery DQuickDciIconImagePrivate::getUrlQuery()
{
    QUrlQuery query;
    query.addQueryItem(QLatin1String("name"), name);
    query.addQueryItem(QLatin1String("mode"), QString::number(mode));
    query.addQueryItem(QLatin1String("theme"), QString::number(theme));
    DDciIconPalette pal = palette;
    if (!palette.foreground().isValid() && q_func()->color().isValid()) {
        pal.setForeground(q_func()->color());
    }
    query.addQueryItem(QLatin1String("palette"), DDciIconPalette::convertToString(pal));
    query.addQueryItem(QLatin1String("devicePixelRatio"), QString::number(devicePixelRatio));

    return query;
}

DQuickDciIconImage::DQuickDciIconImage(QQuickItem *parent)
    : DQuickIconImage(*(new DQuickDciIconImagePrivate), parent)
{
}

DQuickDciIconImage::~DQuickDciIconImage()
{
}

QString DQuickDciIconImage::name() const
{
    Q_D(const DQuickDciIconImage);

    return d->name;
}

DQMLGlobalObject::ControlState DQuickDciIconImage::mode() const
{
    Q_D(const DQuickDciIconImage);

    return d->mode;
}

DQuickDciIconImage::Theme DQuickDciIconImage::theme() const
{
    Q_D(const DQuickDciIconImage);

    return d->theme;
}

void DQuickDciIconImage::setName(const QString &name)
{
    Q_D(DQuickDciIconImage);
    d->name = name;
    d->maybeUpdateUrl();
    Q_EMIT nameChanged();
}

void DQuickDciIconImage::setMode(DQMLGlobalObject::ControlState mode)
{
    Q_D(DQuickDciIconImage);
    d->mode = mode;
    d->maybeUpdateUrl();
    Q_EMIT modeChanged();
}

void DQuickDciIconImage::setTheme(DQuickDciIconImage::Theme theme)
{
    Q_D(DQuickDciIconImage);
    d->theme = theme;
    d->maybeUpdateUrl();
    Q_EMIT themeChanged();
}

void DQuickDciIconImage::setPalette(const DDciIconPalette &palette)
{
    Q_D(DQuickDciIconImage);
    d->palette = palette;
    d->maybeUpdateUrl();
    Q_EMIT paletteChanged();
}

DDciIconPalette DQuickDciIconImage::palette() const
{
    Q_D(const DQuickDciIconImage);
    return d->palette;
}

bool DQuickDciIconImage::isNull(const QString &iconName) const
{
    QString iconPath;
    auto cached = DIconTheme::cached();

    if (cached) {
        iconPath = cached->findDciIconFile(iconName.isEmpty() ? name() : iconName, appIconThemeName());
    } else {
        iconPath = DIconTheme::findDciIconFile(iconName.isEmpty() ? name() : iconName, appIconThemeName());
    }

    return iconPath.isEmpty();
}

DQuickIconAttached *DQuickDciIconImage::qmlAttachedProperties(QObject *object)
{
    auto item = qobject_cast<QQuickItem *>(object);

    if (!item)
        return nullptr;

    return new DQuickIconAttached(item);
}

class DQuickIconAttachedPrivate : public DCORE_NAMESPACE::DObjectPrivate
{
    D_DECLARE_PUBLIC(DQuickIconAttached)
public:
    DQuickIconAttachedPrivate(DCORE_NAMESPACE::DObject *qq)
    : DObjectPrivate(qq)
    {}

    DQMLGlobalObject::ControlState mode = DQMLGlobalObject::NormalState;
    DQuickDciIconImage::Theme theme = DQuickDciIconImage::Light;
    DDciIconPalette palette;
};

DQuickIconAttached::DQuickIconAttached(QQuickItem *parent)
    : QObject(parent)
    , DObject(*new DQuickIconAttachedPrivate(this))
{
}

DQuickIconAttached::~DQuickIconAttached()
{
}

DQMLGlobalObject::ControlState DQuickIconAttached::mode() const
{
    D_DC(DQuickIconAttached);
    return d->mode;
}

void DQuickIconAttached::setMode(DQMLGlobalObject::ControlState mode)
{
    D_D(DQuickIconAttached);
    if (d->mode == mode)
        return;

    d->mode = mode;
    Q_EMIT modeChanged();
}

DQuickDciIconImage::Theme DQuickIconAttached::theme() const
{
    D_DC(DQuickIconAttached);
    return d->theme;
}

void DQuickIconAttached::setTheme(DQuickDciIconImage::Theme theme)
{
    D_D(DQuickIconAttached);
    if (d->theme == theme)
        return;

    d->theme = theme;
    Q_EMIT themeChanged();
}

DDciIconPalette DQuickIconAttached::palette() const
{
    D_DC(DQuickIconAttached);
    return d->palette;
}

void DQuickIconAttached::setPalette(const DDciIconPalette &palette)
{
    D_D(DQuickIconAttached);
    if (d->palette == palette)
        return;

    d->palette = palette;
    Q_EMIT paletteChanged();
}

DQUICK_END_NAMESPACE
