// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#pragma once

#include <QString>

class EnvGuard {
public:
    EnvGuard(const char *name, const QString &value)
    {
        m_name = name;
        m_originValue = qgetenv(m_name);
        qputenv(m_name, value.toUtf8());
    }
    ~EnvGuard()
    {
        qputenv(m_name, m_originValue);
    }
private:
    QByteArray m_originValue;
    const char* m_name = nullptr;
};
