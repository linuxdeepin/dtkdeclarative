// SPDX-FileCopyrightText: 2020 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef DQUICKPALETTE_H
#define DQUICKPALETTE_H

#include <dtkdeclarative_global.h>
#define private public
#include <QPalette>
#undef private

DQUICK_BEGIN_NAMESPACE

class DQuickPalette
{
    Q_GADGET

    // QPalette
    Q_PROPERTY(QColor alternateBase READ alternateBase WRITE setAlternateBase RESET resetAlternateBase FINAL)
    Q_PROPERTY(QColor base READ base WRITE setBase RESET resetBase FINAL)
    Q_PROPERTY(QColor brightText READ brightText WRITE setBrightText RESET resetBrightText FINAL)
    Q_PROPERTY(QColor button READ button WRITE setButton RESET resetButton FINAL)
    Q_PROPERTY(QColor buttonText READ buttonText WRITE setButtonText RESET resetButtonText FINAL)
    Q_PROPERTY(QColor dark READ dark WRITE setDark RESET resetDark FINAL)
    Q_PROPERTY(QColor highlight READ highlight WRITE setHighlight RESET resetHighlight FINAL)
    Q_PROPERTY(QColor highlightedText READ highlightedText WRITE setHighlightedText RESET resetHighlightedText FINAL)
    Q_PROPERTY(QColor light READ light WRITE setLight RESET resetLight FINAL)
    Q_PROPERTY(QColor link READ link WRITE setLink RESET resetLink FINAL)
    Q_PROPERTY(QColor linkVisited READ linkVisited WRITE setLinkVisited RESET resetLinkVisited FINAL)
    Q_PROPERTY(QColor mid READ mid WRITE setMid RESET resetMid FINAL)
    Q_PROPERTY(QColor midlight READ midlight WRITE setMidlight RESET resetMidlight FINAL)
    Q_PROPERTY(QColor shadow READ shadow WRITE setShadow RESET resetShadow FINAL)
    Q_PROPERTY(QColor text READ text WRITE setText RESET resetText FINAL)
    Q_PROPERTY(QColor toolTipBase READ toolTipBase WRITE setToolTipBase RESET resetToolTipBase FINAL)
    Q_PROPERTY(QColor toolTipText READ toolTipText WRITE setToolTipText RESET resetToolTipText FINAL)
    Q_PROPERTY(QColor window READ window WRITE setWindow RESET resetWindow FINAL)
    Q_PROPERTY(QColor windowText READ windowText WRITE setWindowText RESET resetWindowText FINAL)

    // DPalette
    Q_PROPERTY(QColor itemBackground READ itemBackground WRITE setItemBackground RESET resetItemBackground FINAL)
    Q_PROPERTY(QColor textTitle READ textTitle WRITE setTextTitle RESET resetTextTitle FINAL)
    Q_PROPERTY(QColor textTips READ textTips WRITE setTextTips RESET resetTextTips FINAL)
    Q_PROPERTY(QColor textWarning READ textWarning WRITE setTextWarning RESET resetTextWarning FINAL)
    Q_PROPERTY(QColor textLively READ textLively WRITE setTextLively RESET resetTextLively FINAL)
    Q_PROPERTY(QColor lightLively READ lightLively WRITE setLightLively RESET resetLightLively FINAL)
    Q_PROPERTY(QColor darkLively READ darkLively WRITE setLightLively RESET resetLightLively FINAL)
    Q_PROPERTY(QColor frameBorder READ frameBorder WRITE setFrameBorder RESET resetFrameBorder FINAL)
    Q_PROPERTY(QColor placeholderText READ placeholderText WRITE setPlaceholderText RESET resetPlaceholderText FINAL)
    Q_PROPERTY(QColor frameShadowBorder READ frameShadowBorder WRITE setFrameShadowBorder RESET resetFrameShadowBorder FINAL)
    Q_PROPERTY(QColor obviousBackground READ obviousBackground WRITE setObviousBackground RESET resetObviousBackground FINAL)

public:
    QColor alternateBase() const;
    void setAlternateBase(const QColor &color);
    void resetAlternateBase();

    QColor base() const;
    void setBase(const QColor &color);
    void resetBase();

    QColor brightText() const;
    void setBrightText(const QColor &color);
    void resetBrightText();

    QColor button() const;
    void setButton(const QColor &color);
    void resetButton();

    QColor buttonText() const;
    void setButtonText(const QColor &color);
    void resetButtonText();

    QColor dark() const;
    void setDark(const QColor &color);
    void resetDark();

    QColor highlight() const;
    void setHighlight(const QColor &color);
    void resetHighlight();

    QColor highlightedText() const;
    void setHighlightedText(const QColor &color);
    void resetHighlightedText();

    QColor light() const;
    void setLight(const QColor &color);
    void resetLight();

    QColor link() const;
    void setLink(const QColor &color);
    void resetLink();

    QColor linkVisited() const;
    void setLinkVisited(const QColor &color);
    void resetLinkVisited();

    QColor mid() const;
    void setMid(const QColor &color);
    void resetMid();

    QColor midlight() const;
    void setMidlight(const QColor &color);
    void resetMidlight();

    QColor shadow() const;
    void setShadow(const QColor &color);
    void resetShadow();

    QColor text() const;
    void setText(const QColor &color);
    void resetText();

    QColor toolTipBase() const;
    void setToolTipBase(const QColor &color);
    void resetToolTipBase();

    QColor toolTipText() const;
    void setToolTipText(const QColor &color);
    void resetToolTipText();

    QColor window() const;
    void setWindow(const QColor &color);
    void resetWindow();

    QColor windowText() const;
    void setWindowText(const QColor &color);
    void resetWindowText();

    // DPalette
    QColor itemBackground() const;
    void setItemBackground(const QColor &itemBackground);
    void resetItemBackground();

    QColor textTitle() const;
    void setTextTitle(const QColor &textTitle);
    void resetTextTitle();

    QColor textTips() const;
    void setTextTips(const QColor &textTips);
    void resetTextTips();

    QColor textWarning() const;
    void setTextWarning(const QColor &textWarning);
    void resetTextWarning();

    QColor textLively() const;
    void setTextLively(const QColor &textLively);
    void resetTextLively();

    QColor lightLively() const;
    void setLightLively(const QColor &lightLively);
    void resetLightLively();

    QColor darkLively() const;
    void setDarkLively(const QColor &darkLively);
    void resetDarkLively();

    QColor frameBorder() const;
    void setFrameBorder(const QColor &frameBorder);
    void resetFrameBorder();

    QColor placeholderText() const;
    void setPlaceholderText(const QColor &placeholderText);
    void resetPlaceholderText();

    QColor frameShadowBorder() const;
    void setFrameShadowBorder(const QColor &frameShadowBorder);
    void resetFrameShadowBorder();

    QColor obviousBackground() const;
    void setObviousBackground(const QColor &obviousBackground);
    void resetObviousBackground();

private:
    QPalette v;
};

DQUICK_END_NAMESPACE

#endif // DQUICKPALETTE_H
