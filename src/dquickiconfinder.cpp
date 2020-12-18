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

#include "dquickiconprovider.h"
#include "dquickiconfinder.h"
#include "private/dquickiconfinder_p.h"

#include <QPainter>
#include <QUrlQuery>
#include <DPlatformTheme>
#include <DGuiApplicationHelper>

DQUICK_BEGIN_NAMESPACE

DQuickIconFinderPrivate::DQuickIconFinderPrivate(DQuickIconFinder *qq)
    : DTK_CORE_NAMESPACE::DObjectPrivate(qq)
{

}

void DQuickIconFinderPrivate::init()
{
    name = QString();
    state = DQuickIconFinder::State::Off;
    mode = DQuickIconFinder::Mode::Normal;
    size = QSize(16, 16);
    color = QColor();
    icon = QIcon::fromTheme(name);

    url.setScheme("image");
    url.setHost("dtk.icon");
    url.setQuery(getUrlQuery());
}

void DQuickIconFinderPrivate::updateUrl()
{
    url.setQuery(getUrlQuery());
}

QUrlQuery DQuickIconFinderPrivate::getUrlQuery()
{
    D_Q(DQuickIconFinder);

    QUrlQuery query;
    query.addQueryItem("name", name);
    query.addQueryItem("iconThemeName", QIcon::themeName());
    query.addQueryItem("state", QString::number(int(state)));
    query.addQueryItem("mode", QString::number(int(mode)));
    query.addQueryItem("size", QString::number(size.width()) + QString("*") + QString::number(size.height()));
    query.addQueryItem("color", QString::number(quint64(color.rgba64())));
    query.addQueryItem("id",QString::number(reinterpret_cast<quintptr>(q)));

    return query;
}

DQuickIconFinder::DQuickIconFinder(QObject *parent)
    : QObject (parent)
    , DTK_CORE_NAMESPACE::DObject(*new DQuickIconFinderPrivate(this))
{
    D_D(DQuickIconFinder);

    d->init();

    auto platformTheme = DTK_GUI_NAMESPACE::DGuiApplicationHelper::instance()->applicationTheme();
    QObject::connect(platformTheme, &DTK_GUI_NAMESPACE::DPlatformTheme::iconThemeNameChanged,
                     this, &DQuickIconFinder::updateIcon);
}

DQuickIconFinder::~DQuickIconFinder()
{

}

/**
 * @brief DQuickIconFinder::name
 * @return Returns the name of theme icon used in QML Application.
 */
QString DQuickIconFinder::name() const
{
    D_DC(DQuickIconFinder);

    return d->name;
}

/**
 * @brief DQuickIconFinder::setName
 * @param name can set to show the icon used in QML Application.
 */
void DQuickIconFinder::setName(const QString &name)
{
    D_D(DQuickIconFinder);

    if (name == d->name) {
        return;
    }
    d->name = name;
    updateIcon(QByteArray());
    d->updateUrl();

    Q_EMIT nameChanged();
    Q_EMIT urlChanged();
}

/**
 * @brief DQuickIconFinder::state
 * @return Returns the state of theme icon used in QML Application.
 */
DQuickIconFinder::State DQuickIconFinder::state() const
{
    D_DC(DQuickIconFinder);

    return d->state;
}

/**
 * @brief DQuickIconFinder::setState
 * @param state can set to show the icon used in QML Application.
 */
void DQuickIconFinder::setState(const DQuickIconFinder::State state)
{
    D_D(DQuickIconFinder);

    d->state = state;
    d->updateUrl();

    Q_EMIT stateChanged();
    Q_EMIT urlChanged();
}

/**
 * @brief DQuickIconFinder::mode
 * @return Returns the mode of theme icon used in QML Application.
 */
DQuickIconFinder::Mode DQuickIconFinder::mode() const
{
    D_DC(DQuickIconFinder);

    return d->mode;
}

/**
 * @brief DQuickIconFinder::setMode
 * @param mode can set to show the icon used in QML Application.
 */
void DQuickIconFinder::setMode(const DQuickIconFinder::Mode mode)
{
    D_D(DQuickIconFinder);

    d->mode = mode;
    d->updateUrl();

    Q_EMIT modeChanged();
    Q_EMIT urlChanged();
}

/**
 * @brief DQuickIconFinder::size
 * @return Returns the size of theme icon used in QML Application.
 */
QSize DQuickIconFinder::size() const
{
    D_DC(DQuickIconFinder);

    return d->size;
}

/**
 * @brief DQuickIconFinder::setSize
 * @param size can set to show the icon used in QML Application.
 */
void DQuickIconFinder::setSize(const QSize &size)
{
    D_D(DQuickIconFinder);

    d->size = size;
    d->updateUrl();

    Q_EMIT sizeChanged();
    Q_EMIT urlChanged();
}

/**
 * @brief DQuickIconFinder::color
 * @return Returns the color of theme icon used in QML Application.
 */
QColor DQuickIconFinder::color() const
{
    D_DC(DQuickIconFinder);

    return d->color;
}

/**
 * @brief DQuickIconFinder::setColor
 * @param color can set to show the icon used in QML Application.
 */
void DQuickIconFinder::setColor(const QColor &color)
{
    D_D(DQuickIconFinder);

    d->color = color;
    d->updateUrl();

    Q_EMIT colorChanged();
    Q_EMIT urlChanged();
}

/**
 * @brief DQuickIconFinder::updateIcon
 * @param iconThemeName Application icon theme name.
 */
void DQuickIconFinder::updateIcon(QByteArray iconThemeName)
{
    Q_UNUSED(iconThemeName)
    D_D(DQuickIconFinder);

    d->icon = QIcon::fromTheme(d->name);
}

/**
 * @brief DQuickIconFinder::url
 * This property value is assign to url of QML type Image, it contain
 * messages of name, state, mode, size and color, and is used by function
 * DQuickIconProvider::requestPixmap(const QString &id, QSize *size, const QSize &requestedSize)
 * @return Returns the url of Image used in QML Application.
 */
QUrl DQuickIconFinder::url() const
{
    D_DC(DQuickIconFinder);

    return d->url;
}

/**
 * @brief DQuickIconFinder::icon
 * @return Returns the type icon of theme icon used in QML Application.
 */
QIcon DQuickIconFinder::icon() const
{
    D_DC(DQuickIconFinder);

    return d->icon;
}

/**
 * @brief DQuickIconFinder::pixmap
 * @return Returns the type pixmap of theme icon used in QML Application.
 */
QPixmap DQuickIconFinder::pixmap() const
{
    D_DC(DQuickIconFinder);

    const QIcon::Mode qMode = static_cast<QIcon::Mode>(d->mode);
    const QIcon::State qState = static_cast<QIcon::State>(d->state);
    const QSize icon_size = d->icon.actualSize(d->size, qMode, qState);
    QPixmap pixmap(icon_size);
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    painter.setPen(d->color);
     d->icon.paint(&painter, QRect(0, 0, d->size.width(), d->size.height()), Qt::AlignCenter, qMode, qState);

     return pixmap;
}

/**
 * @brief DQuickIconFinder::queryToObject
 * @param query is query field of DQuickIconFinder url property.
 * @return Returns object of DQuickIconFinder which provide theme icon for QML type Image.
 */
DQuickIconFinder *DQuickIconFinder::queryToObject(const QString &query)
{
    QUrlQuery urlQuery(query);
    const QString &id = urlQuery.queryItemValue("id");

    return reinterpret_cast<DQuickIconFinder *>(id.toULongLong());;
}

DQUICK_END_NAMESPACE

