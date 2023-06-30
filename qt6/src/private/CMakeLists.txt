#####################################################################
## qtquickcontrols2basicstyleimplplugin Plugin:
#####################################################################

qt_add_qml_module(dtkdeclarativeprivatesplugin
    PLUGIN_TARGET dtkdeclarativeprivatesplugin
    URI "org.deepin.dtk.private"
    VERSION "1.0"
    QML_FILES
        "ProgressBarImpl.qml"
        "ProgressBarPanel.qml"
        "ColorOverlay.qml"
        "SoftwareColorOverlay.qml"
        "ArrowListViewButton.qml"
    OUTPUT_DIRECTORY
        "${PROJECT_BINARY_DIR}/plugins/org/deepin/dtk/private"
)

target_include_directories(dtkdeclarativeprivatesplugin
PRIVATE
    ${PROJECT_SOURCE_DIR}/src/private
)

target_link_libraries(dtkdeclarativeprivatesplugin
PRIVATE
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Quick
    Qt${QT_VERSION_MAJOR}::CorePrivate
    Qt${QT_VERSION_MAJOR}::QuickPrivate
    Qt${QT_VERSION_MAJOR}::Qml
    Qt${QT_VERSION_MAJOR}::QmlPrivate
)

