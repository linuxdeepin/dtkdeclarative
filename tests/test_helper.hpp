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
#include <QQmlExpression>
#include <QQuickItem>
#include <private/qsgplaintexture_p.h>

#define TEST_OFFSCREEN_SKIP() \
if (qEnvironmentVariable("QT_QPA_PLATFORM") == "offscreen") \
    GTEST_SKIP();

#define TEST_NOTSOFTWARE_SKIP() \
if (QQuickWindow::sceneGraphBackend() != "software") \
        GTEST_SKIP_("Only test `QSGRendererInterface::Software` backend");

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

template<typename T = QQuickItem>
T *findItem(QQuickItem *parent, const QString &objectName = QString(), int index = -1)
{
    const QMetaObject &mo = T::staticMetaObject;
    for (int i = 0; i < parent->childItems().count(); ++i) {
        QQuickItem *item = qobject_cast<QQuickItem*>(parent->childItems().at(i));
        if (!item)
            continue;
        if (mo.cast(item) && (objectName.isEmpty() || item->objectName() == objectName)) {
            if (index != -1) {
                QQmlExpression e(qmlContext(item), item, "index");
                if (e.evaluate().toInt() == index)
                    return static_cast<T*>(item);
            } else {
                return static_cast<T*>(item);
            }
        }
        item = findItem<T>(item, objectName, index);
        if (item)
            return static_cast<T*>(item);
    }

    return nullptr;
}

template<class T = QObject>
class ControlHelper
{
public:
    ControlHelper()
    {
        engine.setImportPathList(QStringList {QString::fromLocal8Bit(QML_PLUGIN_PATH)} + engine.importPathList());
    }
    ControlHelper(const QString &url)
        : ControlHelper()
    {
        load(url);
    }
    bool load(const QString &url)
    {
        clear();

        QQmlComponent component(&engine, QUrl(url), QQmlComponent::PreferSynchronous);
        if (!component.isReady()) {
            qWarning() << "component is not ready" << component.errorString();
            return false;
        }

        auto tmp = component.create();
        if (!tmp) {
            qWarning() << "create object is wrong." << component.errorString();
            return false;
        }

        object = qobject_cast<T *>(tmp);
        return object != nullptr;
    }
    ~ControlHelper()
    {
        clear();
    }
    void clear()
    {
        if (object)
            object->deleteLater();
    }
    QQmlEngine engine;
    T *object = nullptr;
};

template<class T = QQuickItem>
class QuickViewHelper {
public:
    QuickViewHelper()
        : view(new QQuickView)
    {
        view->engine()->setImportPathList(QStringList {QString::fromLocal8Bit(QML_PLUGIN_PATH)} + view->engine()->importPathList());
    }
    QuickViewHelper(const QString &url)
        : QuickViewHelper()
    {
        load(url);
        requestExposed();
    }
    bool load(const QString &url)
    {
        view->setSource(url);

        QQuickItem *rootItem = view->rootObject();

        object = qobject_cast<T *>(rootItem);
        return object != nullptr;

    }
    void requestActivate()
    {
        view->requestActivate();
        QVERIFY(QTest::qWaitForWindowActive(view));
    }
    void requestExposed()
    {
        view->show();
        QVERIFY(QTest::qWaitForWindowExposed(view));
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

namespace TestUtil {
    inline QSGTexture *simpleTexture(const QColor &color = Qt::blue, const QSize &size = QSize(100, 100))
    {
        QImage image(size, QImage::Format_RGB32);
        image.fill(color);
        return QSGPlainTexture::fromImage(image);
    }

    template<class T>
    inline void registerType(const char* type)
    {
        qmlRegisterType<T>("test", 1, 0, type);
    }

    inline bool supportOpengl(const QSGRendererInterface::GraphicsApi ga)
    {
#ifndef QT_NO_OPENGL
        return (ga == QSGRendererInterface::OpenGL
#if QT_VERSION >= QT_VERSION_CHECK(5, 14, 0)
                || ga == QSGRendererInterface::OpenGLRhi
                );
#endif
#endif
        return false;
    }
}
