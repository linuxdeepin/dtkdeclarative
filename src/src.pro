load(dtk_lib)
include(private/private.pri)
TARGET = dtkdeclarative
TEMPLATE = lib
QT += dtkcore dtkgui core quick
CONFIG += internal_module

isEmpty(DTK_QML_APP_PLUGIN_PATH) {
    DTK_QML_APP_PLUGIN_PATH = $$LIB_INSTALL_DIR/$$TARGET/qml-app
}

DEFINES += DTK_QML_APP_PLUGIN_PATH=\\\"'$$DTK_QML_APP_PLUGIN_PATH'\\\"

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

template.files += $$PWD/../examples/qml-app-template
template.path = /usr/share/qtcreator/templates/wizards/projects

INSTALLS += includes target template

CMAKE_CONTENT += "set(DTK_QML_APP_PLUGIN_PATH $${DTK_QML_APP_PLUGIN_PATH})"
MODULE_PRI_CONT += "QT.$${TARGET}.qml_apps = $${DTK_QML_APP_PLUGIN_PATH}"

load(dtk_cmake)
load(dtk_module)
