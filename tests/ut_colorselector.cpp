// SPDX-FileCopyrightText: 2023 - 2026 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include <gtest/gtest.h>

#include "test_helper.hpp"
#include <QQmlEngine>
#include <QQmlComponent>
#include <QQuickItem>
#include <QSignalSpy>
#include <QQmlProperty>
#include <QScopeGuard>

#include "dquickcontrolpalette_p.h"

DQUICK_USE_NAMESPACE

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
    ControlHelper<QQuickItem> helper("qrc:/qml/ColorSelector_GetColorFromProperty.qml");
    const auto cleanup = qScopeGuard([&helper] {
        delete helper.object;
        helper.object = nullptr;
    });
    ASSERT_TRUE(helper.object);

    auto listView = helper.object->property("view").value<QQuickItem*>();
    ASSERT_NE(listView, nullptr);

    QObject *buttonPanel = qvariant_cast<QObject*>(listView->property("buttonPanel"));
    ASSERT_NE(buttonPanel, nullptr);

    QObject *background = buttonPanel->findChild<QQuickItem*>("background");
    ASSERT_NE(background, nullptr);

    QQmlEngine &engine = helper.engine;
    engine.globalObject().setProperty("background", engine.newQObject(background));

    ASSERT_TRUE(QQmlProperty::write(helper.object, "palette.window", QColor(Qt::white)));
    QTest::qWaitFor([&] {
        return background->property("color")
                == engine.evaluate("background.color1.normal.common.color()").toVariant();
    });
    ASSERT_EQ(background->property("color"), engine.evaluate("background.color1.normal.common.color()").toVariant());
}

// Use a template control to exercise palette feedback without unrelated style
// assets. The palette is both an input and an output of the color selector.
static const char paletteFeedbackQml[] =
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
        "import QtQuick\nimport QtQuick.Templates as T\n"
#else
        "import QtQuick 2.11\nimport QtQuick.Templates 2.4 as T\n"
#endif
R"(
import org.deepin.dtk 1.0 as D

T.Button {
    property D.Palette textColor: D.Palette {
        normal: ("red")
        normalDark: ("blue")
    }
    palette.window: "white"
    palette.windowText: D.ColorSelector.textColor
}
)";

TEST_F(ut_ColorSelector, paletteUpdatesAreDeferredAndCoalesced)
{
    QList<QQmlError> warnings;
    ControlHelper<QQuickItem> helper;
    QObject::connect(&helper.engine, &QQmlEngine::warnings, &helper.engine,
                     [&warnings](const QList<QQmlError> &errors) { warnings.append(errors); });
    ASSERT_TRUE(helper.setData(paletteFeedbackQml));
    const auto cleanup = qScopeGuard([&helper] {
        delete helper.object;
        helper.object = nullptr;
    });
    auto selector = qobject_cast<DQuickControlColorSelector *>(
            qmlAttachedPropertiesObject<DQuickControlColorSelector>(helper.object));
    ASSERT_NE(selector, nullptr);
    QCoreApplication::processEvents();
    QSignalSpy themeChanges(selector, &DQuickControlColorSelector::controlThemeChanged);
    QSignalSpy colorChanges(selector, &DQuickControlColorSelector::colorPropertyChanged);

    ASSERT_TRUE(QQmlProperty::write(helper.object, "palette.window", QColor(Qt::black)));
    ASSERT_TRUE(QQmlProperty::write(helper.object, "palette.window", QColor(Qt::white)));
    EXPECT_EQ(themeChanges.count(), 0);
    QCoreApplication::processEvents();
    EXPECT_EQ(themeChanges.count(), 0);
    EXPECT_EQ(colorChanges.count(), 0);

    ASSERT_TRUE(QQmlProperty::write(helper.object, "palette.window", QColor(Qt::black)));
    EXPECT_EQ(themeChanges.count(), 0);
    QCoreApplication::processEvents();
    EXPECT_EQ(themeChanges.count(), 1);
    EXPECT_EQ(QQmlProperty::read(helper.object, "palette.windowText").value<QColor>(), QColor(Qt::blue));

    // The palette writeback must settle rather than continuously queue updates.
    const int settledChanges = colorChanges.count();
    for (int i = 0; i < 5; ++i)
        QCoreApplication::processEvents();
    EXPECT_EQ(colorChanges.count(), settledChanges);
    EXPECT_FALSE(selector->m_controlThemeUpdatePending);
    for (const auto &warning : warnings)
        EXPECT_FALSE(warning.description().contains("Binding loop")) << warning.toString().toStdString();
}

TEST_F(ut_ColorSelector, destroyWithPendingPaletteUpdate)
{
    ControlHelper<QQuickItem> helper;
    ASSERT_TRUE(helper.setData(paletteFeedbackQml));
    QPointer<QObject> selector = qmlAttachedPropertiesObject<DQuickControlColorSelector>(helper.object);
    ASSERT_TRUE(QQmlProperty::write(helper.object, "palette.window", QColor(Qt::black)));
    delete helper.object;
    helper.object = nullptr;
    EXPECT_TRUE(selector.isNull());
    QCoreApplication::processEvents();
}

TEST_F(ut_ColorSelector, styledControlsPaletteFeedback)
{
    for (const QByteArray &type : {QByteArray("Button"), QByteArray("ToolButton"), QByteArray("MenuItem")}) {
        SCOPED_TRACE(type.constData());
        QList<QQmlError> warnings;
        ControlHelper<QQuickItem> helper;
        QObject::connect(&helper.engine, &QQmlEngine::warnings, &helper.engine,
                         [&warnings](const QList<QQmlError> &errors) { warnings.append(errors); });
        ASSERT_TRUE(helper.setData("import QtQuick 2.11\nimport org.deepin.dtk 1.0 as D\nD."
                                  + type + " { text: \"Palette feedback\" }"));
        const auto cleanup = qScopeGuard([&helper] {
            delete helper.object;
            helper.object = nullptr;
        });
        auto selector = qobject_cast<DQuickControlColorSelector *>(
                qmlAttachedPropertiesObject<DQuickControlColorSelector>(helper.object));
        ASSERT_NE(selector, nullptr);
        QSignalSpy changes(selector, &DQuickControlColorSelector::colorPropertyChanged);
        for (bool dark : {false, true}) {
            ASSERT_TRUE(QQmlProperty::write(helper.object, "palette.window",
                                           QColor(dark ? Qt::black : Qt::white)));
            for (bool inactive : {true, false}) {
                selector->setInactived(inactive);
                for (int state = 0; state < 5; ++state) {
                    selector->setHovered(state == 1);
                    selector->setPressed(state == 2);
                    helper.object->setEnabled(state != 3);
                    helper.object->setProperty("checked", state == 4);
                    for (int i = 0; i < 5; ++i)
                        QCoreApplication::processEvents();
                    EXPECT_EQ(QQmlProperty::read(helper.object, "palette.windowText").value<QColor>(),
                              selector->property("textColor").value<QColor>());
                    const int settledChanges = changes.count();
                    QCoreApplication::processEvents();
                    EXPECT_EQ(changes.count(), settledChanges);
                    EXPECT_FALSE(selector->m_controlThemeUpdatePending);
                }
            }
        }
        for (const auto &warning : warnings)
            EXPECT_FALSE(warning.description().contains("Binding loop")) << warning.toString().toStdString();
    }
}
