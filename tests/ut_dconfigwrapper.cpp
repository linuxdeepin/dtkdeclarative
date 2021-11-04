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

#include <gtest/gtest.h>

#include <QSignalSpy>
#include <QTest>

#include "private/dconfigwrapper_p.h"

class ut_DConfigWrapper : public ::testing::Test
{
public:
    virtual void SetUp();
    virtual void TearDown();

    static void SetUpTestCase() {
        const QString &target = QString("%1/opt/apps/%2/files/schemas/configs/%3.json").arg(LocalPrefix, APP_ID, FILE_NAME);

        if (!QFile::exists(QFileInfo(target).path()))
            QDir().mkpath(QFileInfo(target).path());

        QFile::copy(":/data/dconf-example.meta.json", target);

        qputenv("DSG_DCONFIG_FILE_BACKEND_LOCAL_PREFIX", LocalPrefix);
        qputenv("DSG_DCONFIG_BACKEND_TYPE", "FileBackend");
    }
    static void TearDownTestCase() {
        QDir(LocalPrefix).removeRecursively();
    }
    static constexpr char const *LocalPrefix = "/tmp/example";
    static constexpr char const *APP_ID = "tests";
    static constexpr char const *FILE_NAME = "example";

    DConfigWrapper *config;
    QString origiAppId;
};

void ut_DConfigWrapper::SetUp()
{
    config = new DConfigWrapper();
}

void ut_DConfigWrapper::TearDown()
{
    delete config;
}

TEST_F(ut_DConfigWrapper, componentComplete)
{
    config->classBegin();
    config->setName("example");
    config->componentComplete();

    ASSERT_TRUE(config->isValid());
}

TEST_F(ut_DConfigWrapper, setValue)
{
    config->classBegin();
    config->setName("example");
    config->setSubpath("");
    config->componentComplete();

    ASSERT_TRUE(config->keyList().contains("key3"));

    QSignalSpy spy(config, &DConfigWrapper::valueChanged);

    QString newValue = config->value("key3", QString("default")).toString() + QString("abc");
    config->setValue("key3", newValue);

    ASSERT_EQ(config->value("key3").toString(), newValue);

    ASSERT_TRUE(QTest::qWaitFor([&spy](){
        return spy.count() == 1;
    }, 500));
}
