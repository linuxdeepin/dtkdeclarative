// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#pragma once

#include <gtest/gtest.h>

#include <QQmlComponent>
#include <QQmlEngine>
#include <QString>
#include <DGuiApplicationHelper>
#include <QQuickView>
#include <QTest>

#define TEST_OFFSCREEN_SKIP() \
if (qEnvironmentVariable("QT_QPA_PLATFORM") == "offscreen") \
    GTEST_SKIP();

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

template<class T = QObject>
class ControlHeler
{
public:
    ControlHeler(const QString &url)
    {
        engine.setImportPathList(QStringList {QString::fromLocal8Bit(QML_PLUGIN_PATH)} + engine.importPathList());

        component = new QQmlComponent(&engine);
        component->loadUrl(QUrl(url), QQmlComponent::PreferSynchronous);
        if (!component->isReady()) {
            qWarning() << "component is not ready" << component->errorString();
            return;
        }

        auto tmp = component->create();
        if (!tmp)
            qWarning() << "create object is wrong." << component->errorString();

        object = qobject_cast<T *>(tmp);
    }
    ~ControlHeler()
    {
        component->deleteLater();
        if (object)
            object->deleteLater();
    }
    QQmlEngine engine;
    QQmlComponent *component = nullptr;
    T *object = nullptr;
};

template<class T = QQuickItem>
class QuickViewHelper {
public:
    QuickViewHelper(const QString &url)
        : view(new QQuickView)
    {
        view->engine()->setImportPathList(QStringList {QString::fromLocal8Bit(QML_PLUGIN_PATH)} + view->engine()->importPathList());

        view->setSource(url);

        QCOMPARE(view->status(), QQuickView::Ready);
        view->show();
        QVERIFY(QTest::qWaitForWindowExposed(view));

        QQuickItem *rootItem = view->rootObject();
        QVERIFY(rootItem);

        object = qobject_cast<T *>(rootItem);
    }
    void requestActivate()
    {
        view->requestActivate();
        QVERIFY(QTest::qWaitForWindowActive(view));
    }
    QQuickView *view = nullptr;
    T *object = nullptr;
};

DGUI_USE_NAMESPACE
class ThemeTypeGuard {
public:
    ThemeTypeGuard(DGuiApplicationHelper::ColorType expectedType)
    {
        oldType = DGuiApplicationHelper::instance()->themeType();
        DGuiApplicationHelper::instance()->setPaletteType(expectedType);
    }
    ~ThemeTypeGuard()
    {
        DGuiApplicationHelper::instance()->setPaletteType(oldType);
    }
private:
    DGuiApplicationHelper::ColorType oldType;
};
