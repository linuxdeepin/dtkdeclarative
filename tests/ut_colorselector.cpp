// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include <gtest/gtest.h>

#include <QSignalSpy>
#include <QTest>
#include <QQmlEngine>
#include <QQmlComponent>
#include <QQuickItem>

class ut_ColorSelector : public ::testing::Test
{
public:
    virtual void SetUp();
    virtual void TearDown();
};

void ut_ColorSelector::SetUp()
{
}

void ut_ColorSelector::TearDown()
{
}

TEST_F(ut_ColorSelector, getColorFromProperty)
{
    QQmlEngine engine;
    engine.setImportPathList(QStringList {QString::fromLocal8Bit(QML_PLUGIN_PATH)} + engine.importPathList());

    QQmlComponent component(&engine);

    component.loadUrl(QUrl("qrc:/qml/ColorSelector_GetColorFromProperty.qml"), QQmlComponent::PreferSynchronous);
    ASSERT_TRUE(component.isReady());
    auto listView = component.create();
    ASSERT_NE(listView, nullptr);

    QObject *buttonPanel = qvariant_cast<QObject*>(listView->property("buttonPanel"));
    ASSERT_NE(buttonPanel, nullptr);

    QObject *background = buttonPanel->findChild<QQuickItem*>("background");
    ASSERT_NE(background, nullptr);

    engine.globalObject().setProperty("background", engine.newQObject(background));
    ASSERT_EQ(background->property("color"), engine.evaluate("background.color1.normal.common.color()").toVariant());
}
