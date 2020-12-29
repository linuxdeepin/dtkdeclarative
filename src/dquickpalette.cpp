/*
 * Copyright (C) 2020 ~ 2020 Uniontech Technology Co., Ltd.
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

#include "dquickpalette.h"
#include "private/dquickpalette_p.h"

#include <QDebug>
#include <QQuickWindow>

#include <DPlatformTheme>
#include <DGuiApplicationHelper>

DGUI_USE_NAMESPACE
DQUICK_BEGIN_NAMESPACE

#define RETURNCOLOR_BYROLE(_role) \
    { \
        D_DC(DQuickPalette); \
        QPalette::ColorGroup colorGroup = QPalette::Active; \
        if (d->item) { \
            if (!d->item->isEnabled()) { \
                colorGroup = QPalette::Disabled; \
            } \
            if (d->item->window() && !d->item->window()->isActive()) { \
                colorGroup = QPalette::Inactive; \
            } \
        } \
        return d->palette.color(colorGroup, (_role)); \
    }

DQuickPalettePrivate::DQuickPalettePrivate(DQuickPalette *qq)
    : DObjectPrivate(qq)
{

}

void DQuickPalettePrivate::initSourceItem(QQuickItem *old, QQuickItem *item)
{
    D_Q(DQuickPalette);

    // 清理旧的信号连接
    if (old)
        QObject::disconnect(old, SIGNAL(windowChanged(QQuickWindow*)), q, SLOT(_q_onWindowChanged(QQuickWindow*)));

    if (item) {
        QObject::connect(item, SIGNAL(windowChanged(QQuickWindow*)), q, SLOT(_q_onWindowChanged(QQuickWindow*)));
        _q_onWindowChanged(item->window());
    } else {
        _q_onWindowChanged(nullptr);
    }
}

void DQuickPalettePrivate::_q_updatePalette()
{
    D_Q(DQuickPalette);

    if (item && item->window()) {
        palette = DGuiApplicationHelper::instance()->windowPalette(item->window());
    } else {
        palette = DGuiApplicationHelper::instance()->applicationPalette();
    }
    Q_EMIT q->paletteChanged();
}

void DQuickPalettePrivate::_q_onWindowChanged(QQuickWindow *window)
{
    D_Q(DQuickPalette);

    // 清理旧的信号连接
    if (itemWindow) {
        auto theme = DGuiApplicationHelper::instance()->windowTheme(itemWindow);
        QObject::disconnect(theme, SIGNAL(paletteChanged(DPalette)),
                            q, SLOT(_q_updatePalette()));
        QObject::disconnect(theme, SIGNAL(themeNameChanged(QByteArray)),
                            q, SLOT(_q_updatePalette()));
        QObject::disconnect(theme, SIGNAL(activeColorChanged(QColor)),
                            q, SLOT(_q_updatePalette()));
    }

    itemWindow = window;

    if (window) {
        // 断开应用程序级别的信号链接
        QObject::disconnect(DGuiApplicationHelper::instance(),
                            SIGNAL(themeTypeChanged(ColorType)),
                            q, SLOT(_q_updatePalette()));

        // 监听窗口级别的各个与palette相关的属性变化
        auto theme = DGuiApplicationHelper::instance()->windowTheme(window);
        QObject::connect(theme, SIGNAL(paletteChanged(DPalette)),
                         q, SLOT(_q_updatePalette()));
        QObject::connect(theme, SIGNAL(themeNameChanged(QByteArray)),
                         q, SLOT(_q_updatePalette()));
        QObject::connect(theme, SIGNAL(activeColorChanged(QColor)),
                         q, SLOT(_q_updatePalette()));
    } else {
        QObject::connect(DGuiApplicationHelper::instance(),
                         SIGNAL(themeTypeChanged(ColorType)),
                         q, SLOT(_q_updatePalette()),
                         Qt::UniqueConnection);
    }
}

DQuickPalette::DQuickPalette(QObject *parent)
    : QObject(parent)
    , DObject(*new DQuickPalettePrivate(this))
{

}

/*!
 * \~chinese \class DQuickPalette
 *
 * \~chinese \brief DQuickPalette 是 DTk 自定义 QML 类型，用于在 QML 中使用调色板相关功能的类。
 * \~chinese 继承自 QObject,DObject,QQmlParserStatus，需要在 QML 中配合　Item 一起使用,通过
 * \~chinese 属性 sourceItem 可以绑定一个 Item 控件，根据控件状态获取相应调色板调色板的值。
 */
DQuickPalette::~DQuickPalette()
{

}

void DQuickPalette::classBegin()
{
    D_D(DQuickPalette);
    d->componentComplete = false;
}

void DQuickPalette::componentComplete()
{
    D_D(DQuickPalette);

    d->componentComplete = true;
    d->initSourceItem(nullptr, d->item.data());
    d->_q_updatePalette();
}

/*!
 * \~chinese \brief DQuickPalette::sourceItem　sourceItem用于获取　DPalette 绑定的　Item。
 * \~chinese \return sourceItem 返回　DPalette 绑定的　Item。
 */
QQuickItem *DQuickPalette::sourceItem() const
{
    D_DC(DQuickPalette);

    return d->item;
}

/*!
 * \~chinese \brief DQuickPalette::sourceItem　sourceItem用于设置　DPalette 绑定的　Item。
 * \~chinese \par DPalette 需要绑定的　Item。
 */
void DQuickPalette::setSourceItem(QQuickItem *item)
{
    D_D(DQuickPalette);

    if (item == d->item) {
        return;
    }

    if (isComponentComplete()) {
        // 不要交换这段代码与 d->item = item 赋值语句的顺序
        d->initSourceItem(d->item, item);
        d->_q_updatePalette();
    }

    d->item = item;
    Q_EMIT sourceItemChanged(item);
}

/*!
 * \~chinese \brief DQuickPalette::window　window　用于获取　DPalette 的　window 类型的颜色值。
 * \~chinese \return window 返回　DPalette　的 window 类型的颜色值。
 */
QColor DQuickPalette::window() const
{
    RETURNCOLOR_BYROLE(QPalette::Window);
}

/*!
 * \~chinese \brief DQuickPalette::window　windowText　用于获取　DPalette 的　windowText 类型的颜色值。
 * \~chinese \return windowText 返回　DPalette　的 windowText 类型的颜色值。
 */
QColor DQuickPalette::windowText() const
{
    RETURNCOLOR_BYROLE(QPalette::WindowText);
}

/*!
 * \~chinese \brief DQuickPalette::window　base　用于获取　DPalette 的　base 类型的颜色值。
 * \~chinese \return base 返回　DPalette　的 base 类型的颜色值。
 */
QColor DQuickPalette::base() const
{
    RETURNCOLOR_BYROLE(QPalette::Base);
}

/*!
 * \~chinese \brief DQuickPalette::window　text　用于获取　DPalette 的　text 类型的颜色值。
 * \~chinese \return text 返回　DPalette　的 text 类型的颜色值。
 */
QColor DQuickPalette::text() const
{
    RETURNCOLOR_BYROLE(QPalette::Text);
}

/*!
 * \~chinese \brief DQuickPalette::window　alternateBase　用于获取　DPalette 的　alternateBase 类型的颜色值。
 * \~chinese \return alternateBase 返回　DPalette　的 alternateBase 类型的颜色值。
 */
QColor DQuickPalette::alternateBase() const
{
    RETURNCOLOR_BYROLE(QPalette::AlternateBase);
}

/*!
 * \~chinese \brief DQuickPalette::window　button　用于获取　DPalette 的　button 类型的颜色值。
 * \~chinese \return button 返回　DPalette　的 button 类型的颜色值。
 */
QColor DQuickPalette::button() const
{
    RETURNCOLOR_BYROLE(QPalette::Button);
}

/*!
 * \~chinese \brief DQuickPalette::window　buttonText　用于获取　DPalette 的　buttonText 类型的颜色值。
 * \~chinese \return buttonText 返回　DPalette　的 buttonText 类型的颜色值。
 */
QColor DQuickPalette::buttonText() const
{
    RETURNCOLOR_BYROLE(QPalette::ButtonText);
}

/*!
 * \~chinese \brief DQuickPalette::light　light　用于获取　DPalette 的　light 类型的颜色值。
 * \~chinese \return light 返回　DPalette　的 light 类型的颜色值。
 */
QColor DQuickPalette::light() const
{
    RETURNCOLOR_BYROLE(QPalette::Light);
}

/*!
 * \~chinese \brief DQuickPalette::midlight　midlight　用于获取　DPalette 的　midlight 类型的颜色值。
 * \~chinese \return midlight 返回　DPalette　的 midlight 类型的颜色值。
 */
QColor DQuickPalette::midlight() const
{
    RETURNCOLOR_BYROLE(QPalette::Midlight);
}

/*!
 * \~chinese \brief DQuickPalette::dark　dark　用于获取　DPalette 的　dark 类型的颜色值。
 * \~chinese \return dark 返回　DPalette　的 dark 类型的颜色值。
 */
QColor DQuickPalette::dark() const
{
    RETURNCOLOR_BYROLE(QPalette::Dark);
}

/*!
 * \~chinese \brief DQuickPalette::mid　mid　用于获取　DPalette 的　mid 类型的颜色值。
 * \~chinese \return mid 返回　DPalette　的 mid 类型的颜色值。
 */
QColor DQuickPalette::mid() const
{
    RETURNCOLOR_BYROLE(QPalette::Mid);
}

/*!
 * \~chinese \brief DQuickPalette::shadow　shadow　用于获取　DPalette 的　shadow 类型的颜色值。
 * \~chinese \return shadow 返回　DPalette　的 shadow 类型的颜色值。
 */
QColor DQuickPalette::shadow() const
{
    RETURNCOLOR_BYROLE(QPalette::Shadow);
}

/*!
 * \~chinese \brief DQuickPalette::highlight　highlight　用于获取　DPalette 的　highlight 类型的颜色值。
 * \~chinese \return highlight 返回　DPalette　的 highlight 类型的颜色值。
 */
QColor DQuickPalette::highlight() const
{
    RETURNCOLOR_BYROLE(QPalette::Highlight);
}

/*!
 * \~chinese \brief DQuickPalette::highlightedText　highlightedText　用于获取　DPalette 的　highlightedText 类型的颜色值。
 * \~chinese \return highlightedText 返回　DPalette　的 highlightedText 类型的颜色值。
 */
QColor DQuickPalette::highlightedText() const
{
    RETURNCOLOR_BYROLE(QPalette::HighlightedText);
}

/*!
 * \~chinese \brief DQuickPalette::itemBackground　itemBackground　用于获取　DPalette 的　itemBackground 类型的颜色值。
 * \~chinese \return itemBackground 返回　DPalette　的 itemBackground 类型的颜色值。
 */
QColor DQuickPalette::itemBackground() const
{
    RETURNCOLOR_BYROLE(DPalette::ItemBackground);
}

/*!
 * \~chinese \brief DQuickPalette::textTitle　textTitle　用于获取　DPalette 的　textTitle 类型的颜色值。
 * \~chinese \return textTitle 返回　DPalette　的 textTitle 类型的颜色值。
 */
QColor DQuickPalette::textTitle() const
{
    RETURNCOLOR_BYROLE(DPalette::TextTitle);
}

/*!
 * \~chinese \brief DQuickPalette::textTips　textTips　用于获取　DPalette 的　textTips 类型的颜色值。
 * \~chinese \return textTips 返回　DPalette　的 textTips 类型的颜色值。
 */
QColor DQuickPalette::textTips() const
{
    RETURNCOLOR_BYROLE(DPalette::TextTips);
}

/*!
 * \~chinese \brief DQuickPalette::textWarning　textWarning　用于获取　DPalette 的　textWarning 类型的颜色值。
 * \~chinese \return textWarning 返回　DPalette　的 textWarning 类型的颜色值。
 */
QColor DQuickPalette::textWarning() const
{
    RETURNCOLOR_BYROLE(DPalette::TextWarning);
}

/*!
 * \~chinese \brief DQuickPalette::textLively　textLively　用于获取　DPalette 的　textLively 类型的颜色值。
 * \~chinese \return textLively 返回　DPalette　的 textLively 类型的颜色值。
 */
QColor DQuickPalette::textLively() const
{
    RETURNCOLOR_BYROLE(DPalette::TextLively);
}

/*!
 * \~chinese \brief DQuickPalette::lightLively　lightLively　用于获取　DPalette 的　lightLively 类型的颜色值。
 * \~chinese \return lightLively 返回　DPalette　的 lightLively 类型的颜色值。
 */
QColor DQuickPalette::lightLively() const
{
    RETURNCOLOR_BYROLE(DPalette::LightLively);
}

/*!
 * \~chinese \brief DQuickPalette::darkLively　darkLively　用于获取　DPalette 的　darkLively 类型的颜色值。
 * \~chinese \return darkLively 返回　DPalette　的 darkLively 类型的颜色值。
 */
QColor DQuickPalette::darkLively() const
{
    RETURNCOLOR_BYROLE(DPalette::DarkLively);
}

/*!
 * \~chinese \brief DQuickPalette::frameBorder　frameBorder　用于获取　DPalette 的　frameBorder 类型的颜色值。
 * \~chinese \return frameBorder 返回　DPalette　的 frameBorder 类型的颜色值。
 */
QColor DQuickPalette::frameBorder() const
{
    RETURNCOLOR_BYROLE(DPalette::FrameBorder);
}

/*!
 * \~chinese \brief DQuickPalette::placeholderText　placeholderText　用于获取　DPalette 的　placeholderText 类型的颜色值。
 * \~chinese \return placeholderText 返回　DPalette　的 placeholderText 类型的颜色值。
 */
QColor DQuickPalette::placeholderText() const
{
    RETURNCOLOR_BYROLE(DPalette::PlaceholderText);
}

/*!
 * \~chinese \brief DQuickPalette::frameShadowBorder　frameShadowBorder　用于获取　DPalette 的　frameShadowBorder 类型的颜色值。
 * \~chinese \return frameShadowBorder 返回　DPalette　的 frameShadowBorder 类型的颜色值。
 */
QColor DQuickPalette::frameShadowBorder() const
{
    RETURNCOLOR_BYROLE(DPalette::FrameShadowBorder);
}

/*!
 * \~chinese \brief DQuickPalette::obviousBackground　obviousBackground　用于获取　DPalette 的　obviousBackground 类型的颜色值。
 * \~chinese \return obviousBackground 返回　DPalette　的 obviousBackground 类型的颜色值。
 */
QColor DQuickPalette::obviousBackground() const
{
    RETURNCOLOR_BYROLE(DPalette::ObviousBackground);
}


bool DQuickPalette::isComponentComplete() const
{
    D_DC(DQuickPalette);

    return d->componentComplete;
}

DQUICK_END_NAMESPACE

#include "moc_dquickpalette.cpp"
