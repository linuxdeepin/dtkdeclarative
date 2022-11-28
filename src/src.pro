load(dtk_lib)
include(private/private.pri)
TARGET = dtkdeclarative
TEMPLATE = lib
QT += dtkcore dtkgui core quick quick-private
CONFIG += internal_module c++11 qtquickcompiler

# 龙芯架构上没有默认添加PT_GNU_STACK-section,所以此处手动指定一下
contains(QMAKE_HOST.arch, mips.*): QMAKE_LFLAGS_SHLIB += "-Wl,-z,noexecstack"

# for debian
isEmpty(LIB_INSTALL_DIR) {
    LIB_INSTALL_DIR = $$[QT_INSTALL_LIBS]
}

isEmpty(DTK_QML_APP_PLUGIN_SUBPATH) {
    DTK_QML_APP_PLUGIN_SUBPATH = dtkdeclarative/plugins
}

DEFINES += DTK_QML_APP_PLUGIN_SUBPATH=\\\"'$$DTK_QML_APP_PLUGIN_SUBPATH'\\\"

include(src.pri)

includes.files += \
    $$PWD/*.h \
    $$PWD/DQuickWindow \
    $$PWD/DAppLoader \
    $$PWD/DQmlAppMainWindowInterface \
    $$PWD/DQmlAppPreloadInterface \
    $$PWD/DPlatformThemeProxy \
    $$PWD/DQuickItemViewport \
    $$PWD/DQuickBlitFramebuffer \

RESOURCES += $$PWD/dtkdeclarative_assets.qrc

DTK_MODULE_NAME=$$TARGET
load(dtk_build)

template.files += $$PWD/../examples/qml-app-template
template.path = /usr/share/qtcreator/templates/wizards/projects

INSTALLS += includes target template

CMAKE_CONTENT += "include(GNUInstallDirs)"
CMAKE_CONTENT += "set(DTK_QML_APP_PLUGIN_PATH ${CMAKE_INSTALL_FULL_LIBDIR}/$${DTK_QML_APP_PLUGIN_SUBPATH})"
MODULE_PRI_CONT += "QT.$${TARGET}.qml_apps = \$\$[QT_INSTALL_LIBS]/$${DTK_QML_APP_PLUGIN_SUBPATH}"

DEFINES += DTK_QML_APP_PLUGIN_PATH=\\\"'$$[QT_INSTALL_LIBS]/$$DTK_QML_APP_PLUGIN_SUBPATH'\\\"

load(dtk_cmake)
load(dtk_module)

DISTFILES += \
    shaders/quickitemduplicator.frag \
    shaders/quickitemduplicator-opaque.frag

RESOURCES += \
    dtkdeclarative_qml.qrc
