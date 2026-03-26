// SPDX-FileCopyrightText: 2026 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#pragma once

#include <dtkcore_global.h>
#include <dtkdeclarative_global.h>

#include <QQmlParserStatus>
#include <QtQml/qqml.h>

#include <DLicenseInfo>

DQUICK_BEGIN_NAMESPACE
DCORE_USE_NAMESPACE

class DLicenseInfoProvider : public QObject, public QQmlParserStatus
{
    Q_OBJECT
    Q_INTERFACES(QQmlParserStatus)
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    QML_NAMED_ELEMENT(LicenseInfoProvider)
#endif

    Q_PROPERTY(QVariantList licenseList READ licenseList NOTIFY licenseListChanged)
    Q_PROPERTY(QString path READ path WRITE setPath NOTIFY pathChanged)
    Q_PROPERTY(bool valid READ isValid NOTIFY validChanged)

public:
    explicit DLicenseInfoProvider(QObject *parent = nullptr);
    ~DLicenseInfoProvider() override;

    QVariantList licenseList() const;

    // path to a custom license JSON file
    QString path() const;
    void setPath(const QString &path);

    bool isValid() const;
    void setValid(bool valid);

    Q_INVOKABLE QString licenseContent(const QString &licenseName);

Q_SIGNALS:
    void pathChanged();
    void validChanged();
    void licenseListChanged();

private:
    void classBegin() override {};
    void componentComplete() override;

private:
    DLicenseInfo m_licenseInfo;
    QVariantList m_componentInfoList;
    QString m_path;
    bool m_valid = false;
};

DQUICK_END_NAMESPACE
