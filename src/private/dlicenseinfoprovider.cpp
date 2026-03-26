// SPDX-FileCopyrightText: 2026 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include "dlicenseinfoprovider_p.h"

DQUICK_BEGIN_NAMESPACE

DLicenseInfoProvider::DLicenseInfoProvider(QObject *parent)
    : QObject(parent)
{
}

DLicenseInfoProvider::~DLicenseInfoProvider() = default;

QVariantList DLicenseInfoProvider::licenseList() const
{
    auto componentInfos = m_licenseInfo.componentInfos();
    QVariantList list;
    for (const auto &info : componentInfos) {
        QVariantMap map;
        map.insert("name", info->name());
        map.insert("version", info->version());
        map.insert("copyRight", info->copyRight());
        map.insert("licenseName", info->licenseName());
        list.append(map);
    }
    return list;
}

QString DLicenseInfoProvider::path() const
{
    return m_path;
}

void DLicenseInfoProvider::setPath(const QString &path)
{
    if (m_path == path)
        return;
    m_path = path;
    Q_EMIT pathChanged();
    setValid(m_licenseInfo.loadFile(path));
    Q_EMIT licenseListChanged();
}

bool DLicenseInfoProvider::isValid() const
{
    return m_valid;
}

void DLicenseInfoProvider::setValid(bool valid)
{
    if (m_valid == valid)
        return;
    m_valid = valid;
    Q_EMIT validChanged();
}

QString DLicenseInfoProvider::licenseContent(const QString &licenseName)
{
    return QString::fromUtf8(m_licenseInfo.licenseContent(licenseName));
}

void DLicenseInfoProvider::componentComplete()
{
    setValid(m_licenseInfo.loadFile({}));
    Q_EMIT licenseListChanged();
}

DQUICK_END_NAMESPACE
