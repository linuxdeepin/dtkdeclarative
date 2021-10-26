/*
 * Copyright (C) 2020 Uniontech Technology Co., Ltd.
 *
 * Author:     zccrs <zccrs@live.com>
 *
 * Maintainer: zccrs <zhangjide@uniontech.com>
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

#ifndef DQUICKICONIMAGE_P_P_H
#define DQUICKICONIMAGE_P_P_H

#include <private/dquickiconimage_p.h>
#ifndef emit
#define emit Q_EMIT
#include <private/qquickimage_p_p.h>
#undef emit
#else
#include <private/qquickimage_p_p.h>
#endif


DQUICK_BEGIN_NAMESPACE

class DQuickIconImagePrivate : public QQuickImagePrivate
{
    Q_DECLARE_PUBLIC(DQuickIconImage)

public:
    void init();
    void maybeUpdateUrl();
    QUrlQuery getUrlQuery();
    DQuickIconImage::Mode getIconMode() const;

    qreal calculateDevicePixelRatio() const;
    bool updateDevicePixelRatio(qreal targetDevicePixelRatio) override;

    QString name;
    DQuickIconImage::State state = DQuickIconImage::State::Off;
    DQuickIconImage::Mode mode = DQuickIconImage::Mode::Invalid;
    QColor color;

    enum IconType : qint8 {
        ThemeIconName, // 图标名称
        Base64Data, // base64编码的图标图片数据
        FileUrl // 图标文件的url地址
    };

    // 记录此图标是否应该从图标主题中获取。
    // 在freedesktop的规范中，图标可为
    // 一个本地文件，或以base64编码的图片数据。
    // 此处的兼容处理主要是为了让DQuickIconFinder
    // 能直接用于加载desktop文件的图标
    IconType iconType = ThemeIconName;
};

DQUICK_END_NAMESPACE

#endif // DQUICKICONIMAGE_P_P_H
