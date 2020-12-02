load(dtk_lib)
include(private/private.pri)
TARGET = dtkdeclarative
TEMPLATE = lib
QT += dtkcore dtkgui core quick
CONFIG += internal_module

HEADERS += \
    dtkdeclarative_global.h \
    dquickwindow.h

SOURCES += \
    dquickwindow.cpp

includes.files += \
    $$PWD/*.h \
    $$PWD/DQuickWindow

DTK_MODULE_NAME=$$TARGET
load(dtk_build)

INSTALLS += includes target

load(dtk_cmake)
load(dtk_module)

