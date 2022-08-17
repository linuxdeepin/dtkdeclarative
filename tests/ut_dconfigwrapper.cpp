// SPDX-FileCopyrightText: 2021 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include <gtest/gtest.h>

#include <QSignalSpy>
#include <QTest>

#include "private/dconfigwrapper_p.h"

static constexpr char const *LocalPrefix = "/tmp/example";
static constexpr char const *APP_ID = "tests";
static constexpr char const *FILE_NAME = "example";
class ut_DConfigWrapper : public ::testing::Test
{
public:
    virtual void SetUp();
    virtual void TearDown();

    static void SetUpTestCase() {
        const QString &target = QString("%1/usr/share/dsg/configs/%2/%3.json").arg(LocalPrefix, APP_ID, FILE_NAME);

        if (!QFile::exists(QFileInfo(target).path()))
            QDir().mkpath(QFileInfo(target).path());

        QFile::copy(":/data/dconf-example.meta.json", target);

        qputenv("DSG_DCONFIG_FILE_BACKEND_LOCAL_PREFIX", LocalPrefix);
        qputenv("DSG_DCONFIG_BACKEND_TYPE", "FileBackend");
        qputenv("DSG_DATA_DIRS", "/usr/share/dsg");
    }
    static void TearDownTestCase() {
        QDir(LocalPrefix).removeRecursively();
    }

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
