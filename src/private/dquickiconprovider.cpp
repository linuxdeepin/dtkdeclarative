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

#include <QIcon>
#include <QUrlQuery>
#include <QPainter>
#include <DIconTheme>
#include <DDciIcon>
#include <DGuiApplicationHelper>
#include <DPlatformTheme>

#include <private/dquickdciiconimage_p.h>

#include "dquickiconprovider_p.h"
#include "dqmlglobalobject_p.h"

DQUICK_BEGIN_NAMESPACE
DGUI_USE_NAMESPACE

DQuickIconProvider::DQuickIconProvider()
    : QQuickImageProvider(QQuickImageProvider::Image, QQuickImageProvider::ForceAsynchronousImageLoading)
{

}

QImage DQuickIconProvider::requestImage(const QString &id, QSize *size, const QSize &requestedSize)
{
    QUrlQuery urlQuery(id);

    if (!urlQuery.hasQueryItem("name"))
        return QImage();

    QString name = urlQuery.queryItemValue("name");

    if (name.isEmpty())
        return QImage();

    QIcon icon = QIcon::fromTheme(name);

    if (icon.isNull())
        return QImage();

    QIcon::Mode qMode = QIcon::Normal;
    QIcon::State qState = QIcon::Off;

    if (urlQuery.hasQueryItem("mode"))
        qMode = static_cast<QIcon::Mode>(urlQuery.queryItemValue("mode").toInt());

    if (urlQuery.hasQueryItem("state"))
        qState = static_cast<QIcon::State>(urlQuery.queryItemValue("state").toInt());

    // 获取图标的缩放比例
    qreal devicePixelRatio = 1.0;
    if (urlQuery.hasQueryItem("devicePixelRatio")) {
        devicePixelRatio = urlQuery.queryItemValue("devicePixelRatio").toDouble();
        if (qIsNull(devicePixelRatio))
            devicePixelRatio = 1.0;
    }

    QSize icon_size = requestedSize;
    // 初始时可能没有为图标设置期望的大小
    if (icon_size.isEmpty()) {
        icon_size = icon.availableSizes(qMode, qState).first();
    } else {
        // 因为传入的requestedSize是已经乘过缩放的, 因此此处要除以缩放比例获取真实的图标大小
        icon_size /= devicePixelRatio;
    }

    QImage image(icon_size * devicePixelRatio, QImage::Format_ARGB32);
    image.setDevicePixelRatio(devicePixelRatio);
    image.fill(Qt::transparent);
    QPainter painter(&image);

    QColor color(urlQuery.queryItemValue("color"));
    // Fixed filling icon when color is transparent.
    if (color.isValid() && color.alpha() > 0)
        painter.setPen(color);

    // 务必要使用paint的方式获取图片, 有部分可变色的图标类型, 在图标引擎中会使用QPainter::pen的颜色
    // 作为图标的填充颜色
    icon.paint(&painter, QRect(QPoint(0, 0), icon_size), Qt::AlignCenter, qMode, qState);

    if (size)
        *size = icon_size;

    return image;
}

class DQuickDciIconProviderPrivate
{
public:
    QImage requestImageFromIconProvider(const QString &id, QSize *size, const QSize &requestedSize)
    {
        DQuickIconProvider iconProvider;
        return iconProvider.requestImage(id, size, requestedSize);
    }
};

DQuickDciIconProvider::DQuickDciIconProvider()
    : QQuickImageProvider(QQuickImageProvider::Image,
                          QQuickImageProvider::ForceAsynchronousImageLoading)
    , d(new DQuickDciIconProviderPrivate)
{
}

DQuickDciIconProvider::~DQuickDciIconProvider()
{
    delete d;
}

/*!
    \internal
    \brief A function that handles DCI icon for use in QML.

    This provider is added when the dtk qml plugin construction
    and use to generate the icon resource data needed in the \a id.
    This function adds icon of specified size for the different states,
    topics, types, and other attributes needed in the id.

    \note You should not add a wrong DCI icon here.
 */
QImage DQuickDciIconProvider::requestImage(const QString &id, QSize *size, const QSize &requestedSize)
{
    QUrlQuery urlQuery(id);
    if (!urlQuery.hasQueryItem("name"))
        return QImage();

    QString name = urlQuery.queryItemValue("name");
    if (name.isEmpty())
        return QImage();

    QString iconPath;
    if (auto cached = DIconTheme::cached()) {
        iconPath = cached->findDciIconFile(name, DGuiApplicationHelper::instance()->applicationTheme()->iconThemeName());
    } else {
        iconPath = DIconTheme::findDciIconFile(name, DGuiApplicationHelper::instance()->applicationTheme()->iconThemeName());
    }

    if (Q_UNLIKELY(iconPath.isEmpty())) {
        // Fallback to normal qicon.
       return d->requestImageFromIconProvider(id, size, requestedSize);
    }
    DDciIcon dciIcon(iconPath);
    if (dciIcon.isNull())
        return d->requestImageFromIconProvider(id, size, requestedSize);

    DDciIcon::Mode mode = DDciIcon::Normal;
    if (urlQuery.hasQueryItem("mode")) {
        int modeValue = urlQuery.queryItemValue("mode").toInt();
        switch (modeValue) {
        case DQMLGlobalObject::NormalState:
            mode = DDciIcon::Normal;
            break;
        case DQMLGlobalObject::DisabledState:
            mode = DDciIcon::Disabled;
            break;
        case DQMLGlobalObject::HoveredState:
            mode = DDciIcon::Hover;
            break;
        case DQMLGlobalObject::PressedState:
            mode = DDciIcon::Pressed;
            break;
        default:
            break;
        }
    }

    int theme = DQuickDciIconImage::UnknowTheme;
    if (urlQuery.hasQueryItem("theme"))
        theme = static_cast<DDciIcon::Theme>(urlQuery.queryItemValue("theme").toInt());

    if (theme == DQuickDciIconImage::UnknowTheme) {
        QColor window = DGuiApplicationHelper::instance()->applicationPalette().window().color();
        theme = DGuiApplicationHelper::toColorType(window);
    }

    // Get the icon pixel ratio
    qreal devicePixelRatio = 1.0;
    if (urlQuery.hasQueryItem("devicePixelRatio")) {
        devicePixelRatio = urlQuery.queryItemValue("devicePixelRatio").toDouble();
        if (qIsNull(devicePixelRatio))
            devicePixelRatio = 1.0;
    }

    DDciIconPalette palette;
    if (urlQuery.hasQueryItem("palette")) {
        palette = DDciIconPalette::convertFromString(urlQuery.queryItemValue("palette"));
    }

    // If the target mode icon didn't found, we should find the normal mode icon
    // and decorate to the target mode.
    int boundingSize = qMax(requestedSize.width(), requestedSize.height());
    QPixmap pixmap = dciIcon.pixmap(devicePixelRatio, boundingSize, DDciIcon::Theme(theme), mode, palette);
    if (pixmap.isNull())
        pixmap = dciIcon.pixmap(devicePixelRatio, boundingSize, DDciIcon::Theme(theme), DDciIcon::Normal, palette);

    if (pixmap.isNull())
        return QImage();

    QImage image = pixmap.toImage();
    if (size)
        *size = image.size();
    return image;
}

DQUICK_END_NAMESPACE
