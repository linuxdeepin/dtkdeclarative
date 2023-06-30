// SPDX-FileCopyrightText: 2020 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include "qmlplugin_plugin.h"
#include "dquickwindow.h"
#include "dquickitemviewport.h"
#include "dquickblitframebuffer.h"

#include "private/dqmlglobalobject_p.h"
#include "private/dquickiconimage_p.h"
#include "private/dquickdciiconimage_p.h"
#include "private/dquickiconlabel_p.h"

#include "private/dquickimageprovider_p.h"
#include "private/dquickcontrolpalette_p.h"

#include <DFontManager>

#include <QQmlEngine>
#include <QSortFilterProxyModel>
#include <qqml.h>

DGUI_USE_NAMESPACE

DQUICK_BEGIN_NAMESPACE

static const QString softwareBackend = "software";

template<typename T>
inline void dtkRegisterType(const char *uri1, const char *uri2, int versionMajor, int versionMinor, const char *qmlName) {
    qmlRegisterType<T>(uri1, versionMajor, versionMinor, qmlName);
    if (uri2)
        qmlRegisterType<T>(uri2, versionMajor, versionMinor, qmlName);
}
template<typename T>
inline void dtkRegisterAnonymousType(const char *uri1, const char *uri2, int versionMajor) {
#if (QT_VERSION > QT_VERSION_CHECK(5, 14, 0))
    qmlRegisterAnonymousType<T>(uri1, versionMajor);
    if (uri2)
        qmlRegisterAnonymousType<T>(uri2, versionMajor);
#else
    Q_UNUSED(uri1)
    Q_UNUSED(uri2)
    Q_UNUSED(versionMajor)
    qmlRegisterType<T>();
#endif
}
template<typename T>
inline void dtkRegisterUncreatableType(const char *uri1, const char *uri2, int versionMajor, int versionMinor, const char *qmlName, const QString& reason) {
    qmlRegisterUncreatableType<T>(uri1, versionMajor, versionMinor, qmlName, reason);
    if (uri2)
        qmlRegisterUncreatableType<T>(uri2, versionMajor, versionMinor, qmlName, reason);
}
template <typename T>
inline void dtkRegisterSingletonType(const char *uri1, const char *uri2, int versionMajor, int versionMinor, const char *qmlName,
                                     QObject *(*callback)(QQmlEngine *, QJSEngine *)) {
    qmlRegisterSingletonType<T>(uri1, versionMajor, versionMinor, qmlName, callback);
    if (uri2)
        qmlRegisterSingletonType<T>(uri2, versionMajor, versionMinor, qmlName, callback);
}
inline void dtkRegisterType(const char *uri1, const char *uri2, int versionMajor, int versionMinor, const char *qmlName, const char *subdir = "") {
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    static QString urlTemplate = QStringLiteral("qrc:/dtk/declarative/qml/%1%2.qml");
#else
    static QString urlTemplate = QStringLiteral("qrc:/dtk/declarative/qml-qt6/%1%2.qml");
#endif
    const QUrl url(urlTemplate.arg(subdir).arg(qmlName));
    qmlRegisterType(url, uri1, versionMajor, versionMinor, qmlName);
    if (uri2)
        qmlRegisterType(url, uri2, versionMajor, versionMinor, qmlName);
}

inline void dtkRegisterTypeAlias(const char *uri1, const char *uri2, int versionMajor, int versionMinor, const char *qmlFileName, const char *alias, const char *subdir = "") {
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    static QString urlTemplate = QStringLiteral("qrc:/dtk/declarative/qml/%1%2.qml");
#else
    static QString urlTemplate = QStringLiteral("qrc:/dtk/declarative/qml-qt6/%1%2.qml");
#endif
    const QUrl url(urlTemplate.arg(subdir).arg(qmlFileName));
    qmlRegisterType(url, uri1, versionMajor, versionMinor, alias);
    if (uri2)
        qmlRegisterType(url, uri2, versionMajor, versionMinor, alias);
}

inline void dtkStyleRegisterSingletonType(const char *uri1, const char *uri2, int versionMajor, int versionMinor, const char *qmlName) {
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    static QString urlTemplate = QStringLiteral("qrc:/dtk/declarative/qml/style/%1.qml");
#else
    static QString urlTemplate = QStringLiteral("qrc:/dtk/declarative/qml-qt6/style/%1.qml");
#endif
    QUrl url(urlTemplate.arg(qmlName));
    qmlRegisterSingletonType(url, uri1, versionMajor, versionMinor, qmlName);
    if (uri2)
        qmlRegisterSingletonType(url, uri2, versionMajor, versionMinor, qmlName);
}
inline void dtkSettingsRegisterType(const char *uri1, const char *uri2, int versionMajor, int versionMinor, const char *qmlName) {
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    static QString urlTemplate = QStringLiteral("qrc:/dtk/declarative/qml/settings/%1.qml");
#else
    static QString urlTemplate = QStringLiteral("qrc:/dtk/declarative/qml-qt6/settings/%1.qml");
#endif
    const QUrl url(urlTemplate.arg(qmlName));
    qmlRegisterType(url, uri1, versionMajor, versionMinor, qmlName);
    if (uri2)
        qmlRegisterType(url, uri2, versionMajor, versionMinor, qmlName);
}

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
static QVariant quickColorTypeConverter(const QString &data)
{
    return QVariant::fromValue(DQuickControlColor(QColor(data)));
}

static QVariant dcolorTypeConverter(const QString &data)
{
    return QVariant::fromValue(DColor(QColor(data)));
}
#endif

template <typename ReturnType>
ReturnType convertColorToQuickColorType(const QColor &value)
{
    return DQuickControlColor(value);
}

template <typename ReturnType>
ReturnType convertColorToDColorType(const QColor &value)
{
    return DColor(value);
}

template <typename ReturnType>
ReturnType convertDColorToQuickColorType(const DColor &value)
{
    return DQuickControlColor(value);
}

void QmlpluginPlugin::registerTypes(const char *uri)
{
    // @uri org.deepin.dtk
    qmlRegisterModule(uri, 1, 0);
    // @uri org.deepin.dtk.impl
    const QByteArray implUri = QByteArray(uri).append(".impl");
//    qmlRegisterModule(implUri.constData(), 1, 0);
    // @uri org.deepin.dtk.private
    const QByteArray privateUri = QByteArray(uri).append(".private");
//    qmlRegisterModule(privateUri.constData(), 1, 0);
    // @uri org.deepin.dtk.controls
    const QByteArray controlsUri = QByteArray(uri).append(".controls");
//    qmlRegisterModule(controlsUri.constData(), 1, 0);
    // @uri org.deepin.dtk.style
    const QByteArray styleUri = QByteArray(uri).append(".style");
//    qmlRegisterModule(styleUri.constData(), 1, 0);
    // @uri org.deepin.dtk.settings
    const QByteArray settingsUri = QByteArray(uri) + ".settings";
//    qmlRegisterModule(settingsUri, 1, 0);

    if (softwareBackend == QQuickWindow::sceneGraphBackend()) {
        dtkRegisterTypeAlias(uri, implUri, 1, 0, "SoftwareColorOverlay", "ColorOverlay", "private/");
    } else {
        dtkRegisterType(uri, implUri, 1, 0, "ColorOverlay", "private/");
    }

    qRegisterMetaType<DQUICK_NAMESPACE::DQuickDciIcon>();
    qRegisterMetaType<DQuickControlColor>("ControlColor");
    qRegisterMetaType<DColor::Type>();
    qRegisterMetaType<DGUI_NAMESPACE::DDciIconPalette>();

    //DQMLGlobalObject 依赖 DWindowManagerHelper中枚举的定义，所以需要先注册
//    qRegisterMetaType<DGuiApplicationHelper::ColorType>("Dtk::Gui::DGuiApplicationHelper::ColorType");

    // 自定义的 QML 控件可以通过把 QML 文件注册到环境中的方式来实现
    // for org.deepin.dtk

    // for org.deepin.dtk.controls
    // QtQuick Controls
    // TODO(xiaoyaobing): software rendering has not been completed
    if (softwareBackend == QQuickWindow::sceneGraphBackend()) {
        dtkRegisterTypeAlias(uri, implUri, 1, 0, "SoftwareOpacityMask", "OpacityMask", "private/");
    } else {
        dtkRegisterType(uri, implUri, 1, 0, "OpacityMask", "private/");
    }

//     for org.deepin.dtk.style(allowed to override)
//    dtkStyleRegisterSingletonType(uri, styleUri, 1, 0, "Style");

//    // for org.deepin.dtk.settings
//    dtkRegisterType<SettingsOption>(settingsUri, implUri, 1, 0, "SettingsOption");
//    dtkRegisterType<SettingsGroup>(settingsUri, implUri, 1, 0, "SettingsGroup");
//    dtkRegisterType<SettingsContainer>(settingsUri, implUri, 1, 0, "SettingsContainer");
//    dtkSettingsRegisterType(settingsUri, nullptr, 1, 0, "SettingsDialog");
//    dtkSettingsRegisterType(settingsUri, nullptr, 1, 0, "OptionDelegate");
//    dtkSettingsRegisterType(settingsUri, nullptr, 1, 0, "CheckBox");
//    dtkSettingsRegisterType(settingsUri, nullptr, 1, 0, "LineEdit");
//    dtkSettingsRegisterType(settingsUri, nullptr, 1, 0, "ComboBox");
//    dtkSettingsRegisterType(settingsUri, nullptr, 1, 0, "NavigationTitle");
//    dtkSettingsRegisterType(settingsUri, nullptr, 1, 0, "ContentTitle");
//    dtkSettingsRegisterType(settingsUri, nullptr, 1, 0, "ContentBackground");

    // for org.deepin.dtk.private
//    dtkRegisterType(privateUri, implUri, 1, 0, "ButtonPanel");

    // for custom type
    QMetaType::registerConverter<QColor, DQuickControlColor>(convertColorToQuickColorType<DQuickControlColor>);
    QMetaType::registerConverter<QColor, DColor>(convertColorToDColorType<DColor>);
    QMetaType::registerConverter<DColor, DQuickControlColor>(convertDColorToQuickColorType<DQuickControlColor>);
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QQmlMetaType::registerCustomStringConverter(qMetaTypeId<DQuickControlColor>(), quickColorTypeConverter);
    QQmlMetaType::registerCustomStringConverter(qMetaTypeId<DColor>(), dcolorTypeConverter);
#endif
    qmlRegisterModuleImport(uri, QQmlModuleImportModuleAny, implUri, 1);
}

void QmlpluginPlugin::initializeEngine(QQmlEngine *engine, const char *uri)
{
    Q_UNUSED(DGuiApplicationHelper::instance());
    engine->addImageProvider("dtk.icon", new DQuickIconProvider);
    engine->addImageProvider("dtk.dci.icon", new DQuickDciIconProvider);
    engine->addImageProvider("dtk.shadow", new DQuickShadowProvider);
    QQmlExtensionPlugin::initializeEngine(engine, uri);
    DQMLGlobalObject::loadTranslator();
}

DQUICK_END_NAMESPACE
