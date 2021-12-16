/*
 * Copyright (C) 2020 ~ 2020 Deepin Technology Co., Ltd.
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

#include "qmlplugin_plugin.h"
#include "dquickwindow.h"
#include "dqmlglobalobject.h"
#include "dquickitemviewport.h"
#include "dquickiconprovider.h"
#include "dquicksystempalette.h"
#include "dhandlecontextmenuwindow.h"
#include "dquickblitframebuffer.h"
#include "dquickshadow.h"
#include "dquickinwindowblendblur.h"

#include "private/dconfigwrapper_p.h"
#include "private/dquickiconimage_p.h"
#include "private/dquickiconlabel_p.h"
#include "private/dquickbusyindicator_p.h"

#include <DFontManager>

#include <QQmlEngine>
#include <qqml.h>

DGUI_USE_NAMESPACE

DQUICK_BEGIN_NAMESPACE

void QmlpluginPlugin::registerTypes(const char *uri)
{
    qmlRegisterModule(uri, 1, 0);
    // @uri org.deepin.dtk
    qmlRegisterType<DQuickIconImage>(uri, 1, 0, "Icon");
    qmlRegisterType<DQuickIconLabel>(uri, 1, 0, "IconLabel");
    qmlRegisterType<DQuickBusyIndicator>(uri, 1, 0, "BusyIndicator");
    qmlRegisterType<DQuickShadow>(uri, 1, 0, "ShadowEffect");

    //DQMLGlobalObject 依赖 DWindowManagerHelper中枚举的定义，所以需要先注册
    qmlRegisterSingletonType<DWindowManagerHelper>(uri, 1, 0, "WindowManagerHelper", [](QQmlEngine *, QJSEngine *) -> QObject * {
        auto helper = DWindowManagerHelper::instance();
        QQmlEngine::setObjectOwnership(helper, QQmlEngine::CppOwnership);
        return helper;
    });
    qmlRegisterSingletonType<DGuiApplicationHelper>(uri, 1, 0, "ApplicationHelper", [](QQmlEngine *, QJSEngine *) -> QObject * {
        auto helper = DGuiApplicationHelper::instance();
        QQmlEngine::setObjectOwnership(helper, QQmlEngine::CppOwnership);
        return helper;
    });
    qRegisterMetaType<DGuiApplicationHelper::ColorType>("Dtk::Gui::DGuiApplicationHelper::ColorType");
    qmlRegisterSingletonType<DQMLGlobalObject>(uri, 1, 0, "DTK", [](QQmlEngine *, QJSEngine *) -> QObject * {
        return new DQMLGlobalObject;
    });
    qmlRegisterType<DQuickItemViewport>(uri, 1, 0, "ItemViewport");
    qmlRegisterType<DFontManager>(uri, 1, 0, "FontManager");
    qmlRegisterType<DQuickSystemPalette>(uri, 1, 0, "SystemPalette");
    qmlRegisterUncreatableType<DQuickWindow>(uri, 1, 0, "Window", "DQuickWindow Attached");
    qmlRegisterType<DHandleContextMenuWindow>(uri, 1, 0, "ContextMenuWindow");
    qmlRegisterType<DQuickBlitFramebuffer>(uri, 1, 0, "BlitFramebuffer");
    qmlRegisterType<DConfigWrapper>(uri, 1, 0, "Config");
    qmlRegisterType<DQuickInWindowBlendBlur>(uri, 1, 0, "InWindowBlendBlur");

    // 自定义的 QML 控件可以通过把 QML 文件注册到环境中的方式来实现
    qmlRegisterType(QUrl(QStringLiteral("qrc:/dtk/declarative/qml/LineEdit.qml")), uri, 1, 0, "LineEdit");
    qmlRegisterType(QUrl(QStringLiteral("qrc:/dtk/declarative/qml/SearchEdit.qml")), uri, 1, 0, "SearchEdit");
    qmlRegisterType(QUrl(QStringLiteral("qrc:/dtk/declarative/qml/PasswordEdit.qml")), uri, 1, 0, "PasswordEdit");
    qmlRegisterType(QUrl(QStringLiteral("qrc:/dtk/declarative/qml/WarningButton.qml")), uri, 1, 0, "WarningButton");
    qmlRegisterType(QUrl(QStringLiteral("qrc:/dtk/declarative/qml/SuggestButton.qml")), uri, 1, 0, "SuggestButton");
    qmlRegisterType(QUrl(QStringLiteral("qrc:/dtk/declarative/qml/ToolButton.qml")), uri, 1, 0, "ToolButton");
    qmlRegisterType(QUrl(QStringLiteral("qrc:/dtk/declarative/qml/IconButton.qml")), uri, 1, 0, "IconButton");
    qmlRegisterType(QUrl(QStringLiteral("qrc:/dtk/declarative/qml/FloatingButton.qml")), uri, 1, 0, "FloatingButton");
    qmlRegisterType(QUrl(QStringLiteral("qrc:/dtk/declarative/qml/ButtonBoxButton.qml")), uri, 1, 0, "ButtonBoxButton");
    qmlRegisterType(QUrl(QStringLiteral("qrc:/dtk/declarative/qml/ButtonBox.qml")), uri, 1, 0, "ButtonBox");
    qmlRegisterType(QUrl(QStringLiteral("qrc:/dtk/declarative/qml/RectangleBorder.qml")), uri, 1, 0, "RectangleBorder");
    qmlRegisterType(QUrl(QStringLiteral("qrc:/dtk/declarative/qml/CicleSpreadAnimation.qml")), uri, 1, 0, "CicleSpreadAnimation");
    qmlRegisterType(QUrl(QStringLiteral("qrc:/dtk/declarative/qml/TitleBar.qml")), uri, 1, 0, "TitleBar");
    qmlRegisterType(QUrl(QStringLiteral("qrc:/dtk/declarative/qml/DialogTitleBar.qml")), uri, 1, 0, "DialogTitleBar");
    qmlRegisterType(QUrl(QStringLiteral("qrc:/dtk/declarative/qml/WindowMinButton.qml")), uri, 1, 0, "WindowMinButton");
    qmlRegisterType(QUrl(QStringLiteral("qrc:/dtk/declarative/qml/WindowMaxButton.qml")), uri, 1, 0, "WindowMaxButton");
    qmlRegisterType(QUrl(QStringLiteral("qrc:/dtk/declarative/qml/WindowRestoreButton.qml")), uri, 1, 0, "WindowRestoreButton");
    qmlRegisterType(QUrl(QStringLiteral("qrc:/dtk/declarative/qml/WindowCloseButton.qml")), uri, 1, 0, "WindowCloseButton");
    qmlRegisterType(QUrl(QStringLiteral("qrc:/dtk/declarative/qml/WindowQuitFullButton.qml")), uri, 1, 0, "WindowQuitFullButton");
    qmlRegisterType(QUrl(QStringLiteral("qrc:/dtk/declarative/qml/WindowOptionButton.qml")), uri, 1, 0, "WindowOptionButton");
    qmlRegisterType(QUrl(QStringLiteral("qrc:/dtk/declarative/qml/WindowButtonGroup.qml")), uri, 1, 0, "WindowButtonGroup");
    qmlRegisterType(QUrl(QStringLiteral("qrc:/dtk/declarative/qml/DialogWindow.qml")), uri, 1, 0, "DialogWindow");
    qmlRegisterType(QUrl(QStringLiteral("qrc:/dtk/declarative/qml/AboutDialog.qml")), uri, 1, 0, "AboutDialog");
    qmlRegisterType(QUrl(QStringLiteral("qrc:/dtk/declarative/qml/HelpAction.qml")), uri, 1, 0, "HelpAction");
    qmlRegisterType(QUrl(QStringLiteral("qrc:/dtk/declarative/qml/AboutAction.qml")), uri, 1, 0, "AboutAction");
    qmlRegisterType(QUrl(QStringLiteral("qrc:/dtk/declarative/qml/QuitAction.qml")), uri, 1, 0, "QuitAction");
    qmlRegisterType(QUrl(QStringLiteral("qrc:/dtk/declarative/qml/ThemeMenu.qml")), uri, 1, 0, "ThemeMenu");
    qmlRegisterType(QUrl(QStringLiteral("qrc:/dtk/declarative/qml/DropShadow.qml")), uri, 1, 0, "DropShadow");
}

void QmlpluginPlugin::initializeEngine(QQmlEngine *engine, const char *uri)
{
    engine->addImageProvider("dtk.icon", new DQuickIconProvider);
    QQmlExtensionPlugin::initializeEngine(engine, uri);
}

DQUICK_END_NAMESPACE
