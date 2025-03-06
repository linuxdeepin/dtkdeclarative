// SPDX-FileCopyrightText: 2021 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef DCONFIGWRAPPER_H
#define DCONFIGWRAPPER_H

#include <dtkdeclarative_global.h>
#include <QQmlParserStatus>
#include <QtQml/qqml.h>

DCORE_BEGIN_NAMESPACE
class DConfig;
class DThreadUtils;
DCORE_END_NAMESPACE

class DConfigWrapperMetaObject;
class DConfigWrapper : public QObject, public QQmlParserStatus
{
    Q_OBJECT
    Q_INTERFACES(QQmlParserStatus)
    Q_PROPERTY(QString name READ name WRITE setName)
    Q_PROPERTY(QString subpath READ subpath WRITE setSubpath)
    Q_PROPERTY(bool async READ async WRITE setAsync)
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    QML_NAMED_ELEMENT(Config)
#endif
public:
    explicit DConfigWrapper(QObject *parent = nullptr);
    ~DConfigWrapper() override;

    QString name() const;
    void setName(const QString &name);

    QString subpath() const;
    void setSubpath(const QString &subpath);

    bool async() const;
    void setAsync(bool newAsync);

public Q_SLOTS:
    QVariant value(const QString &key, const QVariant &fallback = QVariant()) const;
    void setValue(const QString &key, const QVariant &value);
    void resetValue(const QString &key);
    QStringList keyList() const;
    bool isValid() const;
    bool isDefaultValue(const QString &key) const;

Q_SIGNALS:
    void valueChanged(const QString &key, const QVariant &value);
    void initialized();

private:
    virtual void classBegin() override;
    virtual void componentComplete() override;

    void initializeProperties() const;

    friend DConfigWrapperMetaObject;
    DConfigWrapperMetaObject *mo = nullptr;
    std::unique_ptr<DTK_CORE_NAMESPACE::DConfig> impl;
    QStringList configKeyList;
    // If the key was set value, add it to the list
    QStringList nonDefaultValueKeyList;
    QMap<QByteArray, QVariant> initializeConfigs;

    QString m_name;
    QString m_subpath;
    bool m_async = false;
    Q_DISABLE_COPY(DConfigWrapper)
};

#endif // DCONFIGWRAPPER_H
