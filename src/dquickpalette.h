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

#ifndef DQUICKPALETTE_H
#define DQUICKPALETTE_H

#include <dtkdeclarative_global.h>

#include <QColor>
#include <QQuickItem>
#include <QQmlParserStatus>

#include <DObject>

DQUICK_BEGIN_NAMESPACE

class DQuickPalettePrivate;
class DQuickPalette : public QObject, public DTK_CORE_NAMESPACE::DObject, public QQmlParserStatus
{
    Q_OBJECT
    Q_INTERFACES(QQmlParserStatus)

    Q_PROPERTY(QQuickItem * sourceItem READ sourceItem WRITE setSourceItem NOTIFY sourceItemChanged)

    // QPalette
    Q_PROPERTY(QColor window READ window NOTIFY paletteChanged)
    Q_PROPERTY(QColor windowText READ windowText NOTIFY paletteChanged)
    Q_PROPERTY(QColor base READ base NOTIFY paletteChanged)
    Q_PROPERTY(QColor text READ text NOTIFY paletteChanged)
    Q_PROPERTY(QColor alternateBase READ alternateBase NOTIFY paletteChanged)
    Q_PROPERTY(QColor button READ button NOTIFY paletteChanged)
    Q_PROPERTY(QColor buttonText READ buttonText NOTIFY paletteChanged)
    Q_PROPERTY(QColor light READ light NOTIFY paletteChanged)
    Q_PROPERTY(QColor midlight READ midlight NOTIFY paletteChanged)
    Q_PROPERTY(QColor dark READ dark NOTIFY paletteChanged)
    Q_PROPERTY(QColor mid READ mid NOTIFY paletteChanged)
    Q_PROPERTY(QColor shadow READ shadow NOTIFY paletteChanged)
    Q_PROPERTY(QColor highlight READ highlight NOTIFY paletteChanged)
    Q_PROPERTY(QColor highlightedText READ highlightedText NOTIFY paletteChanged)

    // DPalette
    Q_PROPERTY(QColor itemBackground READ itemBackground NOTIFY paletteChanged)
    Q_PROPERTY(QColor textTitle READ textTitle NOTIFY paletteChanged)
    Q_PROPERTY(QColor textTips READ textTips NOTIFY paletteChanged)
    Q_PROPERTY(QColor textWarning READ textWarning NOTIFY paletteChanged)
    Q_PROPERTY(QColor textLively READ textLively NOTIFY paletteChanged)
    Q_PROPERTY(QColor lightLively READ lightLively NOTIFY paletteChanged)
    Q_PROPERTY(QColor darkLively READ darkLively NOTIFY paletteChanged)
    Q_PROPERTY(QColor frameBorder READ frameBorder NOTIFY paletteChanged)
    Q_PROPERTY(QColor placeholderText READ placeholderText NOTIFY paletteChanged)
    Q_PROPERTY(QColor frameShadowBorder READ frameShadowBorder NOTIFY paletteChanged)
    Q_PROPERTY(QColor obviousBackground READ obviousBackground NOTIFY paletteChanged)

public:
    DQuickPalette(QObject *parent = Q_NULLPTR);
    ~DQuickPalette() override;

    void classBegin() override;
    void componentComplete() override;

    QQuickItem * sourceItem() const;
    void setSourceItem(QQuickItem *);

    QColor window() const;
    QColor windowText() const;
    QColor base() const;
    QColor text() const;
    QColor alternateBase() const;
    QColor button() const;
    QColor buttonText() const;
    QColor light() const;
    QColor midlight() const;
    QColor dark() const;
    QColor mid() const;
    QColor shadow() const;
    QColor highlight() const;
    QColor highlightedText() const;

    QColor itemBackground() const;
    QColor textTitle() const;
    QColor textTips() const;
    QColor textWarning() const;
    QColor textLively() const;
    QColor lightLively() const;
    QColor darkLively() const;
    QColor frameBorder() const;
    QColor placeholderText() const;
    QColor frameShadowBorder() const;
    QColor obviousBackground() const;

Q_SIGNALS:
    void sourceItemChanged(QQuickItem *item);
    void paletteChanged();

protected:
    bool isComponentComplete() const;

private:
    D_DECLARE_PRIVATE(DQuickPalette)
    D_PRIVATE_SLOT(void _q_updatePalette())
    D_PRIVATE_SLOT(void _q_onWindowChanged(QQuickWindow *window))
};

DQUICK_END_NAMESPACE

#endif // DQUICKPALETTE_H
