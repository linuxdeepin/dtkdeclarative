// SPDX-FileCopyrightText: 2020 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include "dquickiconimage_p.h"
#include "dquickiconimage_p_p.h"

#include <DGuiApplicationHelper>
#include <DPlatformTheme>
#include <QTimer>
#include <QUrlQuery>

DGUI_USE_NAMESPACE
DQUICK_BEGIN_NAMESPACE

bool DQuickIconImagePrivate::updateDevicePixelRatio(qreal targetDevicePixelRatio)
{
    devicePixelRatio = targetDevicePixelRatio > 1.0 ? targetDevicePixelRatio : calculateDevicePixelRatio();
    return true;
}

void DQuickIconImagePrivate::init()
{
    D_Q(DQuickIconImage);

    if (iconType == ThemeIconName) {
        // 强制确保图标的缩放比例正确
        updateDevicePixelRatio(1.0);

        QObject::connect(DGuiApplicationHelper::instance()->applicationTheme(), SIGNAL(iconThemeNameChanged(QByteArray)),
                         q, SLOT(maybeUpdateUrl()));
        // dtk build-in 类型的图标支持区分窗口主题色, 此处需在主题类型变化时更新图标
        QObject::connect(DGuiApplicationHelper::instance(), SIGNAL(themeTypeChanged(ColorType)),
                         q, SLOT(maybeUpdateUrl()));
    } else {
        QObject::disconnect(DGuiApplicationHelper::instance()->applicationTheme(), SIGNAL(iconThemeNameChanged(QByteArray)),
                            q, SLOT(maybeUpdateUrl()));
        QObject::disconnect(DGuiApplicationHelper::instance(), SIGNAL(themeTypeChanged(ColorType)),
                            q, SLOT(maybeUpdateUrl()));
    }

    // 更新url地址
    maybeUpdateUrl();
}

void DQuickIconImagePrivate::maybeUpdateUrl()
{
    D_Q(DQuickIconImage);

    // 不要为非主题中的图标更新url地址
    if (iconType != ThemeIconName)
        return;

    // 当图标名为空视为清理图片内容
    if (name.isEmpty()) {
        if (fallbackSource.isValid())
            q->setSource(fallbackSource);
        return;
    }

    QUrl url;
    url.setScheme("image");
    url.setHost("dtk.icon");
    url.setQuery(getUrlQuery());
    q->setSource(url);
}

QUrlQuery DQuickIconImagePrivate::getUrlQuery()
{
    QUrlQuery query;
    query.addQueryItem("name", name);
    query.addQueryItem("themeName", QIcon::themeName());
    query.addQueryItem("themeType", QString::number(int(DGuiApplicationHelper::instance()->themeType())));
    query.addQueryItem("state", QString::number(int(state)));
    query.addQueryItem("mode", QString::number(int(getIconMode())));

    if (color.isValid())
        query.addQueryItem("color", color.name(QColor::HexArgb));

    query.addQueryItem("devicePixelRatio", QString::number(devicePixelRatio));

    return query;
}

DQuickIconImage::Mode DQuickIconImagePrivate::getIconMode() const
{
    D_QC(DQuickIconImage);

    if (mode != DQuickIconImage::Mode::Invalid)
        return mode;

    if (!q->isEnabled()) {
        return DQuickIconImage::Mode::Disabled;
    }

    return DQuickIconImage::Mode::Normal;
}

qreal DQuickIconImagePrivate::calculateDevicePixelRatio() const
{
    Q_Q(const DQuickIconImage);
    return q->window() ? q->window()->effectiveDevicePixelRatio() : qApp->devicePixelRatio();
}

DQuickIconImage::DQuickIconImage(QQuickItem *parent)
    : QQuickImage(*(new DQuickIconImagePrivate), parent)
{

}

DQuickIconImage::~DQuickIconImage()
{

}

void DQuickIconImage::componentComplete()
{
    QQuickImage::componentComplete();
    D_D(DQuickIconImage);
    // 初始化信号链接和url地址
    d->init();
}

/**
 * @brief DQuickIconImage::name
 * @return Returns the name of theme icon used in QML Application.
 */
QString DQuickIconImage::name() const
{
    D_DC(DQuickIconImage);

    return d->name;
}

/**
 * @brief DQuickIconImage::setName
 * @param name can set to show the icon used in QML Application.
 */
void DQuickIconImage::setName(const QString &name)
{
    D_D(DQuickIconImage);

    if (name == d->name) {
        return;
    }
    d->name = name;
    Q_EMIT nameChanged();

    // 分析icon的类型
    d->iconType = DQuickIconImagePrivate::ThemeIconName;

    if (name.startsWith("data:image/")) {
        // TODO(zccrs): 此处可能为base64编码的图片数据
        d->iconType = DQuickIconImagePrivate::Base64Data;
    } else if (name.indexOf(":/") >= 0) {
        QUrl url(name);

        // 如果name指定的是一个url，则直接将其当作url使用
        if (url.isValid()) {
            d->iconType = DQuickIconImagePrivate::FileUrl;
            setSource(url);
        }
    }

    if (isComponentComplete()) {
        d->init();
    }
}

/**
 * @brief DQuickIconImage::state
 * @return Returns the state of theme icon used in QML Application.
 */
DQuickIconImage::State DQuickIconImage::state() const
{
    D_DC(DQuickIconImage);

    return d->state;
}

/**
 * @brief DQuickIconImage::setState
 * @param state can set to show the icon used in QML Application.
 */
void DQuickIconImage::setState(State state)
{
    D_D(DQuickIconImage);

    if (d->state == state)
        return;

    d->state = state;
    Q_EMIT stateChanged();

    // 尝试重设图标的url地址
    d->maybeUpdateUrl();
}

/**
 * @brief DQuickIconImage::mode
 * @return Returns the mode of theme icon used in QML Application.
 */
DQuickIconImage::Mode DQuickIconImage::mode() const
{
    D_DC(DQuickIconImage);

    return d->mode;
}

/**
 * @brief DQuickIconImage::setMode
 * @param mode can set to show the icon used in QML Application.
 */
void DQuickIconImage::setMode(Mode mode)
{
    D_D(DQuickIconImage);

    if (d->mode == mode)
        return;

    d->mode = mode;
    Q_EMIT modeChanged();

    // 尝试重设图标的url地址
    d->maybeUpdateUrl();
}

/**
 * @brief DQuickIconImage::color
 * @return Returns the color of theme icon used in QML Application.
 */
QColor DQuickIconImage::color() const
{
    D_DC(DQuickIconImage);
    return d->color;
}

/**
 * @brief DQuickIconImage::setColor
 * @param color can set to show the icon used in QML Application.
 */
void DQuickIconImage::setColor(const QColor &color)
{
    D_D(DQuickIconImage);
    if (d->color == color)
        return;

    d->color = color;
    Q_EMIT colorChanged();

    // 尝试重设图标的url地址
    d->maybeUpdateUrl();
}

const QUrl &DQuickIconImage::fallbackSource() const
{
    D_DC(DQuickIconImage);
    return d->fallbackSource;
}

void DQuickIconImage::setFallbackSource(const QUrl &newSource)
{
    D_D(DQuickIconImage);
    if (d->fallbackSource == newSource)
        return;
    d->fallbackSource = newSource;
    Q_EMIT fallbackSourceChanged();

    // 尝试重设图标的url地址
    d->maybeUpdateUrl();
}

DQuickIconImage::DQuickIconImage(DQuickIconImagePrivate &dd, QQuickItem *parent)
    : QQuickImage(dd, parent)
{
}

void DQuickIconImage::itemChange(ItemChange change, const ItemChangeData &value)
{
    Q_D(DQuickIconImage);

    switch (change) {
    case ItemDevicePixelRatioHasChanged: Q_FALLTHROUGH();
    case ItemEnabledHasChanged: {
        // ###!(Chen Bin): When the program exits, it will be called
        // again, but the engine has been freed, causing subsequent
        // functions to crash when loading the image provider registered
        // in engine.
        if (!qmlEngine(this))
            break;

        d->maybeUpdateUrl();
    }
        break;
    default:
        break;
    }

    QQuickImage::itemChange(change, value);
}

void DQuickIconImage::pixmapChange()
{
    // QQuickImage中只会在设置了souceSize的前提下才会计算图片自身的缩放比例
    // 此处强制确保图标的缩放比例正确
    D_D(DQuickIconImage);
    if (d->iconType == DQuickIconImagePrivate::ThemeIconName)
        d->updateDevicePixelRatio(1.0);

    QQuickImage::pixmapChange();

    if (status() == Error && d->fallbackSource.isValid()) {
        // fallback to source property
        setSource(d->fallbackSource);
    }
}

DQUICK_END_NAMESPACE

#include "moc_dquickiconimage_p.cpp"
