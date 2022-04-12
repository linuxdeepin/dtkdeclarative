TEMPLATE = lib
# qtquickcompiler 是为qrc中的qml文件自动生成qmlc
CONFIG += plugin qtquickcompiler

TARGET = qtquickcontrols2chameleonstyleplugin
TARGETPATH = QtQuick/Controls.2/Chameleon
DESTDIR = $$PWD/imports/Chameleon

QT += qml quick quick-private dtkgui
qtHaveModule(quickcontrols2-private) {
    QT += quickcontrols2-private
    DEFINES += USE_QQuickStylePlugin
}

unix: LIBS += -L$$OUT_PWD/../src -ldtkdeclarative

CONFIG(debug, debug|release) {
    unix: QMAKE_RPATHDIR += $$OUT_PWD/../src
}
INCLUDEPATH += $$PWD/../src

include($$PWD/src/src.pri)
include($$PWD/imports/Chameleon/qml.pri)

# Install *.so and qml files
unix {
    target.path = $$[QT_INSTALL_QML]/$$TARGETPATH

    # Qmldir file and qml files
    qmlfiles.base = $$_PRO_FILE_PWD_/imports/Chameleon
    qmlfiles.files = $$JS_FILES $$QML_FILES
    qmlfiles.path = $${target.path}

    qmldir.input += $${qmlfiles.base}/qmldir.in
    # 改名回qmldir，本地的qmldir主要是用于examples项目直接加载本地chameleon主题，不要将它安装到系统中
    qmldir.output = $$OUT_PWD/qmldir
    QMAKE_SUBSTITUTES += qmldir
    qmlfiles.files += $$OUT_PWD/qmldir

    INSTALLS += target qmlfiles
    OTHER_FILES += $${qmlfiles.base}/qmldir.in
}

# 自动生成qmlc文件
static {
    message("QML cache generation ahead of time is not supported in static builds")
    return()
}

qtPrepareTool(QML_CACHEGEN, qmlcachegen, _ARCH_CHECK)

isEmpty(TARGETPATH): error("Must set TARGETPATH (QML import name) for ahead-of-time QML cache generation")
# 将qmlc文件跟qml文件放到同一个路径下
QMLCACHE_DESTDIR = ./

CACHEGEN_FILES=
qmlcacheinst.files =
for(qmlf, QML_FILES) {
    contains(qmlf,.*\\.js$)|contains(qmlf,.*\\.qml$) {
        CACHEGEN_FILES += $$absolute_path($$qmlf, $$_PRO_FILE_PWD_)
        qmlcacheinst.files += $$QMLCACHE_DESTDIR/$$relative_path($$qmlf, $$_PRO_FILE_PWD_)c
    }
}

defineReplace(qmlCacheOutputFileName) {
    return($$relative_path($$QMLCACHE_DESTDIR/$$relative_path($$1, $$_PRO_FILE_PWD_)c, $$OUT_PWD))
}

qmlcacheinst.base = $$QMLCACHE_DESTDIR
qmlcacheinst.path = $$[QT_INSTALL_QML]/$$TARGETPATH
qmlcacheinst.CONFIG = no_check_exist

qmlcachegen.input = CACHEGEN_FILES
qmlcachegen.output = ${QMAKE_FUNC_FILE_IN_qmlCacheOutputFileName}
qmlcachegen.CONFIG = no_link target_predeps
qmlcachegen.commands = $$QML_CACHEGEN -o ${QMAKE_FILE_OUT} ${QMAKE_FILE_IN}
qmlcachegen.name = Generate QML Cache ${QMAKE_FILE_IN}
qmlcachegen.variable_out = GENERATED_FILES

!debug_and_release|!build_all|CONFIG(release, debug|release) {
    QMAKE_EXTRA_COMPILERS += qmlcachegen
    INSTALLS += qmlcacheinst
}
