/*
 * Copyright (C) 2022 deepin Technology Co., Ltd.
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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef DQUICKWATERPROGRESSATTRIBUTE_P_H
#define DQUICKWATERPROGRESSATTRIBUTE_P_H

#include <dtkdeclarative_global.h>

#include <DObject>
#include <QQmlListProperty>
#include <QObject>
#include <QSharedDataPointer>

class QQuickItem;
DQUICK_BEGIN_NAMESPACE
class WaterPopAttributePrivate;
class WaterPopAttribute : public QObject
{
    Q_OBJECT
    Q_PROPERTY(qreal width READ width WRITE setWidth NOTIFY widthChanged FINAL)
    Q_PROPERTY(qreal height READ height WRITE setHeight NOTIFY heightChanged FINAL)
    Q_PROPERTY(qreal xSpeed READ xSpeed WRITE setXSpeed NOTIFY xSpeedChanged FINAL)
    Q_PROPERTY(qreal ySpeed READ ySpeed WRITE setYSpeed NOTIFY ySpeedChanged FINAL)
    Q_PROPERTY(qreal x READ x WRITE setX NOTIFY xChanged FINAL)
    Q_PROPERTY(qreal y READ y WRITE setY NOTIFY yChanged FINAL)

public:
    WaterPopAttribute(qreal sizeRatio = 0, qreal xSpeed = 0, qreal ySpeed = 0);
    ~WaterPopAttribute();

    WaterPopAttribute(const WaterPopAttribute &other);
    WaterPopAttribute& operator=(const WaterPopAttribute &other);

    qreal xSpeed() const;
    void setXSpeed(qreal popXSpeed);

    qreal ySpeed() const;
    void setYSpeed(qreal popYSpeed);

    qreal x() const;
    void setX(qreal x);

    qreal y() const;
    void setY(qreal y);

    qreal width() const;
    void setWidth(qreal width);

    qreal height() const;
    void setHeight(qreal height);

    qreal sizeRatio() const;

    inline qreal yOffset() const;
    inline void setYOffset(qreal offset);

Q_SIGNALS:
    void xSpeedChanged();
    void ySpeedChanged();
    void xChanged();
    void yChanged();
    void widthChanged();
    void heightChanged();

private:
    QSharedDataPointer<WaterPopAttributePrivate> d;
};

class DQuickWaterProgressAttributePrivate;
class DQuickWaterProgressAttribute : public QObject, DCORE_NAMESPACE::DObject
{
    Q_OBJECT
    D_DECLARE_PRIVATE(DQuickWaterProgressAttribute)
    Q_PROPERTY(qreal frontXOffset READ frontXOffset NOTIFY frontXOffsetChanged)
    Q_PROPERTY(qreal backXOffset READ backXOffset NOTIFY backXOffsetChanged)
    Q_PROPERTY(qreal imageWidth READ imageWidth WRITE setImageWidth NOTIFY imageWidthChanged)
    Q_PROPERTY(qreal imageHeight READ imageHeight WRITE setImageHeight NOTIFY imageHeightChanged)
    Q_PROPERTY(QQmlListProperty<DTK_QUICK_NAMESPACE::WaterPopAttribute> pops READ pops NOTIFY popsChanged)
    Q_PROPERTY(bool running READ running WRITE setRunning NOTIFY runningChanged)
    Q_PROPERTY(QQuickItem *waterProgress READ waterProgress WRITE setWaterProgress NOTIFY waterProgressChanged)

public:
    explicit DQuickWaterProgressAttribute(QObject *parent = nullptr);
    ~DQuickWaterProgressAttribute() {}

    qreal frontXOffset() const;
    void setFrontXOffset(qreal frontXOffset);

    qreal backXOffset() const;
    void setBackXOffset(qreal backXOffset);

    QQmlListProperty<WaterPopAttribute> pops();

    bool running() const;
    void setRunning(bool running);

    QQuickItem * waterProgress() const;
    void setWaterProgress(QQuickItem * waterProgress);

    qreal imageWidth() const;
    void setImageWidth(qreal imageWidth);

    qreal imageHeight() const;
    void setImageHeight(qreal imageHeight);

Q_SIGNALS:
    void frontXOffsetChanged();
    void backXOffsetChanged();
    void popsChanged();
    void runningChanged();
    void waterProgressChanged();
    void imageWidthChanged();
    void imageHeightChanged();
};

DQUICK_END_NAMESPACE
#endif // DQUICKWATERPROGRESSATTRIBUTE_H
