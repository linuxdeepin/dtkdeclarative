TEMPLATE = lib

TARGET = qtquickcontrols2chameleonstyleplugin
TARGETPATH = QtQuick/Controls.2/Chameleon

QT += qml quick

include($$PWD/src/src.pri)
include($$PWD/qml/qml.pri)

# Install *.so and qml files
unix {
    target.path = $$[QT_INSTALL_QML]/$$TARGETPATH

    # Qmldir file and qml files
    qmlfiles.base = $$_PRO_FILE_PWD_
    qmlfiles.files = $$QML_FILES $$PWD/qmldir
    qmlfiles.path = $${target.path}

    INSTALLS += target qmlfiles
}
