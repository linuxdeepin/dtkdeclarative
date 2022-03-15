/*
 * Copyright (C) 2021 UnionTech Technology Co., Ltd.
 *
 * Author:     yeshanshan <yeshanshan@uniontech.com>
 *
 * Maintainer: yeshanshan <yeshanshan@uniontech.com>
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

#ifndef DCONFIGWRAPPER_H
#define DCONFIGWRAPPER_H

#include <dtkdeclarative_global.h>
#include <QQmlParserStatus>

DCORE_BEGIN_NAMESPACE
class DConfig;
DCORE_END_NAMESPACE

class DConfigWrapperMetaObject;
class DConfigWrapper : public QObject, public QQmlParserStatus
{
    Q_OBJECT
    Q_INTERFACES(QQmlParserStatus)
    Q_PROPERTY(QString name READ name WRITE setName)
    Q_PROPERTY(QString subpath READ subpath WRITE setSubpath)
public:
    explicit DConfigWrapper(QObject *parent = nullptr);
    ~DConfigWrapper() override;

    QString name() const;
    void setName(const QString &name);

    QString subpath() const;
    void setSubpath(const QString &subpath);

public Q_SLOTS:
    QVariant value(const QString &key, const QVariant &fallback = QVariant()) const;
    void setValue(const QString &key, const QVariant &value);
    void resetValue(const QString &key);
    QStringList keyList() const;
    bool isValid() const;

Q_SIGNALS:
    void valueChanged(const QString &key);

public:
    virtual void classBegin() override;
    virtual void componentComplete() override;

private:
    friend DConfigWrapperMetaObject;
    DTK_CORE_NAMESPACE::DConfig *impl;
    QString m_name;
    QString m_subpath;
    Q_DISABLE_COPY(DConfigWrapper)
};

#endif // DCONFIGWRAPPER_H
