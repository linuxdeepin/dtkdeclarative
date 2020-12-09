load(dtk_lib)
include(private/private.pri)
TARGET = dtkdeclarative
TEMPLATE = lib
QT += dtkcore dtkgui core quick
CONFIG += internal_module

!isEmpty(DTK_QML_APP_PLUGIN_PATH) {
    DEFINES += DTK_QML_APP_PLUGIN_PATH=\\\"'$$DTK_QML_APP_PLUGIN_PATH'\\\"
} else {
    DEFINES += DTK_QML_APP_PLUGIN_PATH=\\\"'$$LIB_INSTALL_DIR/$$TARGET/qml-app'\\\"
}

HEADERS += \
    dtkdeclarative_global.h \
    dquickwindow.h \
    dapploader.h \
    dqmlappplugininterface.h

SOURCES += \
    dquickwindow.cpp \
    dapploader.cpp

includes.files += \
    $$PWD/*.h \
    $$PWD/DQuickWindow \
    $$PWD/DAppLoader \
    $$PWD/DQmlAppPluginInterface

DTK_MODULE_NAME=$$TARGET
load(dtk_build)

INSTALLS += includes target

load(dtk_cmake)
load(dtk_module)

