// SPDX-FileCopyrightText: 2020 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include "dquickpalette_p.h"
#include "private/dquicksystempalette_p.h"

#include <DPalette>
#include <DGuiApplicationHelper>

#include <QDebug>

DGUI_USE_NAMESPACE
DQUICK_BEGIN_NAMESPACE

QColor DQuickPalette::alternateBase() const
{
    return v.color(QPalette::AlternateBase);
}

void DQuickPalette::setAlternateBase(const QColor &color)
{
    v.setColor(QPalette::All, QPalette::AlternateBase, color);
}

void DQuickPalette::resetAlternateBase()
{
    v.resolve(v.resolve() & ~(1 << QPalette::AlternateBase));
}

QColor DQuickPalette::base() const
{
    return v.color(QPalette::Base);
}

void DQuickPalette::setBase(const QColor &color)
{
    v.setColor(QPalette::All, QPalette::Base, color);
}

void DQuickPalette::resetBase()
{
    v.resolve(v.resolve() & ~(1 << QPalette::Base));
}

QColor DQuickPalette::brightText() const
{
    return v.color(QPalette::BrightText);
}

void DQuickPalette::setBrightText(const QColor &color)
{
    v.setColor(QPalette::All, QPalette::BrightText, color);
}

void DQuickPalette::resetBrightText()
{
    v.resolve(v.resolve() & ~(1 << QPalette::BrightText));
}

QColor DQuickPalette::button() const
{
    return v.color(QPalette::Button);
}

void DQuickPalette::setButton(const QColor &color)
{
    v.setColor(QPalette::All, QPalette::Button, color);
}

void DQuickPalette::resetButton()
{
    v.resolve(v.resolve() & ~(1 << QPalette::Button));
}

QColor DQuickPalette::buttonText() const
{
    return v.color(QPalette::ButtonText);
}

void DQuickPalette::setButtonText(const QColor &color)
{
    v.setColor(QPalette::All, QPalette::ButtonText, color);
}

void DQuickPalette::resetButtonText()
{
    v.resolve(v.resolve() & ~(1 << QPalette::ButtonText));
}

QColor DQuickPalette::dark() const
{
    return v.color(QPalette::Dark);
}

void DQuickPalette::setDark(const QColor &color)
{
    v.setColor(QPalette::All, QPalette::Dark, color);
}

void DQuickPalette::resetDark()
{
    v.resolve(v.resolve() & ~(1 << QPalette::Dark));
}

QColor DQuickPalette::highlight() const
{
    return v.color(QPalette::Highlight);
}

void DQuickPalette::setHighlight(const QColor &color)
{
    v.setColor(QPalette::All, QPalette::Highlight, color);
}

void DQuickPalette::resetHighlight()
{
    v.resolve(v.resolve() & ~(1 << QPalette::Highlight));
}

QColor DQuickPalette::highlightedText() const
{
    return v.color(QPalette::HighlightedText);
}

void DQuickPalette::setHighlightedText(const QColor &color)
{
    v.setColor(QPalette::All, QPalette::HighlightedText, color);
}

void DQuickPalette::resetHighlightedText()
{
    v.resolve(v.resolve() & ~(1 << QPalette::HighlightedText));
}

QColor DQuickPalette::light() const
{
    return v.color(QPalette::Light);
}

void DQuickPalette::setLight(const QColor &color)
{
    v.setColor(QPalette::All, QPalette::Light, color);
}

void DQuickPalette::resetLight()
{
    v.resolve(v.resolve() & ~(1 << QPalette::Light));
}

QColor DQuickPalette::link() const
{
    return v.color(QPalette::Link);
}

void DQuickPalette::setLink(const QColor &color)
{
    v.setColor(QPalette::All, QPalette::Link, color);
}

void DQuickPalette::resetLink()
{
    v.resolve(v.resolve() & ~(1 << QPalette::Link));
}

QColor DQuickPalette::linkVisited() const
{
    return v.color(QPalette::LinkVisited);
}

void DQuickPalette::setLinkVisited(const QColor &color)
{
    v.setColor(QPalette::All, QPalette::LinkVisited, color);
}

void DQuickPalette::resetLinkVisited()
{
    v.resolve(v.resolve() & ~(1 << QPalette::LinkVisited));
}

QColor DQuickPalette::mid() const
{
    return v.color(QPalette::Mid);
}

void DQuickPalette::setMid(const QColor &color)
{
    v.setColor(QPalette::All, QPalette::Mid, color);
}

void DQuickPalette::resetMid()
{
    v.resolve(v.resolve() & ~(1 << QPalette::Mid));
}

QColor DQuickPalette::midlight() const
{
    return v.color(QPalette::Midlight);
}

void DQuickPalette::setMidlight(const QColor &color)
{
    v.setColor(QPalette::All, QPalette::Midlight, color);
}

void DQuickPalette::resetMidlight()
{
    v.resolve(v.resolve() & ~(1 << QPalette::Midlight));
}

QColor DQuickPalette::shadow() const
{
    return v.color(QPalette::Shadow);
}

void DQuickPalette::setShadow(const QColor &color)
{
    v.setColor(QPalette::All, QPalette::Shadow, color);
}

void DQuickPalette::resetShadow()
{
    v.resolve(v.resolve() & ~(1 << QPalette::Shadow));
}

QColor DQuickPalette::text() const
{
    return v.color(QPalette::Text);
}

void DQuickPalette::setText(const QColor &color)
{
    v.setColor(QPalette::All, QPalette::Text, color);
}

void DQuickPalette::resetText()
{
    v.resolve(v.resolve() & ~(1 << QPalette::Text));
}

QColor DQuickPalette::toolTipBase() const
{
    return v.color(QPalette::ToolTipBase);
}

void DQuickPalette::setToolTipBase(const QColor &color)
{
    v.setColor(QPalette::All, QPalette::ToolTipBase, color);
}

void DQuickPalette::resetToolTipBase()
{
    v.resolve(v.resolve() & ~(1 << QPalette::ToolTipBase));
}

QColor DQuickPalette::toolTipText() const
{
    return v.color(QPalette::ToolTipText);
}

void DQuickPalette::setToolTipText(const QColor &color)
{
    v.setColor(QPalette::All, QPalette::ToolTipText, color);
}

void DQuickPalette::resetToolTipText()
{
    v.resolve(v.resolve() & ~(1 << QPalette::ToolTipText));
}

QColor DQuickPalette::window() const
{
    return v.color(QPalette::Window);
}

void DQuickPalette::setWindow(const QColor &color)
{
    v.setColor(QPalette::All, QPalette::Window, color);
}

void DQuickPalette::resetWindow()
{
    v.resolve(v.resolve() & ~(1 << QPalette::Window));
}

QColor DQuickPalette::windowText() const
{
    return v.color(QPalette::WindowText);
}

void DQuickPalette::setWindowText(const QColor &color)
{
    v.setColor(QPalette::All, QPalette::WindowText, color);
}

void DQuickPalette::resetWindowText()
{
    v.resolve(v.resolve() & ~(1 << QPalette::WindowText));
}

// 用来存储DPalette的颜色数据, 需要注意的是, 此处不可存放指针, 否则可能会导致内存泄露.
// 此对象会存储到QPalette的d指针中, 确保能在QPalette销毁时自动销毁
struct DPaletteData
{
public:
    // 用来记录某个颜色值是否被设置
    qint32 resolve_mask = 0;
    QColor br[DPalette::NColorTypes];
};

// 返回DPalette中的颜色数据
static inline DPaletteData *getDPalette(QPalette &palette, bool autoCreate = false)
{
    Q_UNUSED(palette)

    if (Q_UNLIKELY(autoCreate)) {
        qWarning() << "Not support set color for DPalette";
    }

    // TODO(zccrs): 暂时没有必要实现DPalette中扩展的几个颜色类型的写操作
    // 目前只需要能从qml中读取即可
    return nullptr;
}

inline static const QColor &getDColor(const QPalette &palette, DPalette::ColorType type) {
    auto dpalette = getDPalette(const_cast<QPalette&>(palette));
    if (Q_UNLIKELY(dpalette && dpalette->resolve_mask & (1 << type)))
        return dpalette->br[type];

    auto colorType = DGuiApplicationHelper::toColorType(palette);
    // 否则应当从程序全局中获取数据
    if (Q_LIKELY(colorType == DGuiApplicationHelper::instance()->themeType())) {
        static DQuickSystemPalette *global = new DQuickSystemPalette();
        return global->palette().color(palette.currentColorGroup(), type);
    }

    return DGuiApplicationHelper::standardPalette(colorType).color(palette.currentColorGroup(), type);
}

inline static void setDColor(QPalette &palette, DPalette::ColorType type, const QColor &color) {
    auto dpalette = getDPalette(palette, true);
    Q_ASSERT(dpalette);

    dpalette->br[type] = color;
    dpalette->resolve_mask |= (1 << type);
    palette.setColor(QPalette::NoRole, QColor(QRgb(dpalette->resolve_mask)));
}

inline static void resetDColor(QPalette &palette, DPalette::ColorType type) {
    if (auto dpalette = getDPalette(palette)) {
        dpalette->resolve_mask &= ~(1 << type);
        palette.setColor(QPalette::NoRole, QColor(QRgb(dpalette->resolve_mask)));
    }
}

QColor DQuickPalette::itemBackground() const
{
    return getDColor(v, DPalette::ItemBackground);
}

void DQuickPalette::setItemBackground(const QColor &ItemBackground)
{
    setDColor(v, DPalette::ItemBackground, ItemBackground);
}

void DQuickPalette::resetItemBackground()
{
    resetDColor(v, DPalette::ItemBackground);
}

QColor DQuickPalette::textTitle() const
{
    return getDColor(v, DPalette::TextTitle);
}

void DQuickPalette::setTextTitle(const QColor &textTitle)
{
    setDColor(v, DPalette::TextTitle, textTitle);
}

void DQuickPalette::resetTextTitle()
{
    resetDColor(v, DPalette::TextTitle);
}

QColor DQuickPalette::textTips() const
{
    return getDColor(v, DPalette::TextTips);
}

void DQuickPalette::setTextTips(const QColor &textTips)
{
    setDColor(v, DPalette::TextTips, textTips);
}

void DQuickPalette::resetTextTips()
{
    resetDColor(v, DPalette::TextTips);
}

QColor DQuickPalette::textWarning() const
{
    return getDColor(v, DPalette::TextWarning);
}

void DQuickPalette::setTextWarning(const QColor &textWarning)
{
    setDColor(v, DPalette::TextWarning, textWarning);
}

void DQuickPalette::resetTextWarning()
{
    resetDColor(v, DPalette::TextWarning);
}

QColor DQuickPalette::textLively() const
{
    return getDColor(v, DPalette::TextLively);
}

void DQuickPalette::setTextLively(const QColor &textLively)
{
    setDColor(v, DPalette::TextLively, textLively);
}

void DQuickPalette::resetTextLively()
{
    resetDColor(v, DPalette::TextLively);
}

QColor DQuickPalette::lightLively() const
{
    return getDColor(v, DPalette::LightLively);
}

void DQuickPalette::setLightLively(const QColor &lightLively)
{
    setDColor(v, DPalette::LightLively, lightLively);
}

void DQuickPalette::resetLightLively()
{
    resetDColor(v, DPalette::LightLively);
}

QColor DQuickPalette::darkLively() const
{
    return getDColor(v, DPalette::DarkLively);
}

void DQuickPalette::setDarkLively(const QColor &darkLively)
{
    setDColor(v, DPalette::DarkLively, darkLively);
}

void DQuickPalette::resetDarkLively()
{
    resetDColor(v, DPalette::DarkLively);
}

QColor DQuickPalette::frameBorder() const
{
    return getDColor(v, DPalette::FrameBorder);
}

void DQuickPalette::setFrameBorder(const QColor &frameBorder)
{
    setDColor(v, DPalette::FrameBorder, frameBorder);
}

void DQuickPalette::resetFrameBorder()
{
    resetDColor(v, DPalette::FrameBorder);
}

QColor DQuickPalette::placeholderText() const
{
    return getDColor(v, DPalette::PlaceholderText);
}

void DQuickPalette::setPlaceholderText(const QColor &placeholderText)
{
    setDColor(v, DPalette::PlaceholderText, placeholderText);
}

void DQuickPalette::resetPlaceholderText()
{
    resetDColor(v, DPalette::PlaceholderText);
}

QColor DQuickPalette::frameShadowBorder() const
{
    return getDColor(v, DPalette::FrameShadowBorder);
}

void DQuickPalette::setFrameShadowBorder(const QColor &frameShadowBorder)
{
    setDColor(v, DPalette::FrameShadowBorder, frameShadowBorder);
}

void DQuickPalette::resetFrameShadowBorder()
{
    resetDColor(v, DPalette::FrameShadowBorder);
}

QColor DQuickPalette::obviousBackground() const
{
    return getDColor(v, DPalette::ObviousBackground);
}

void DQuickPalette::setObviousBackground(const QColor &obviousBackground)
{
    setDColor(v, DPalette::ObviousBackground, obviousBackground);
}

void DQuickPalette::resetObviousBackground()
{
    resetDColor(v, DPalette::ObviousBackground);
}

DQUICK_END_NAMESPACE
