TEMPLATE = lib
CONFIG += plugin qtquickcompiler

TARGET = qtquickcontrols2chameleonstyleplugin
TARGETPATH = QtQuick/Controls.2/Chameleon

QT += qml quick
qtHaveModule(quickcontrols2-private) {
    QT += quickcontrols2-private
    DEFINES += USE_QQuickStylePlugin
}

RESOURCES += chameleon.qrc

include($$PWD/src/src.pri)
include($$PWD/qml.pri)

# Install *.so and qml files
unix {
    target.path = $$[QT_INSTALL_QML]/$$TARGETPATH

    # Qmldir file and qml files
    qmlfiles.base = $$_PRO_FILE_PWD_
    qmlfiles.files = $$JS_FILES $$QML_FILES $$PWD/qmldir
    qmlfiles.path = $${target.path}

    INSTALLS += target qmlfiles
}
