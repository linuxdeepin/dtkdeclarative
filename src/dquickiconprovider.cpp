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
#include <QDebug>

#include "dquickiconprovider.h"

DQUICK_BEGIN_NAMESPACE

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
    if (!icon_size.isValid()) {
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
    if (color.isValid())
        painter.setPen(color);

    // 务必要使用paint的方式获取图片, 有部分可变色的图标类型, 在图标引擎中会使用QPainter::pen的颜色
    // 作为图标的填充颜色
    icon.paint(&painter, QRect(QPoint(0, 0), icon_size), Qt::AlignCenter, qMode, qState);

    if (size)
        *size = icon_size;

    return image;
}

DQUICK_END_NAMESPACE
