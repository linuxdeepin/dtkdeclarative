#####################################################################
## qtquickcontrols2basicstyleimplplugin Plugin:
#####################################################################

qt_add_qml_module(dtkdeclarativesettingsplugin
    PLUGIN_TARGET dtkdeclarativesettingsplugin
    URI "org.deepin.dtk.settings"
    VERSION "1.0"
    SOURCES
        ${PROJECT_SOURCE_DIR}/src/private/dsettingscontainer_p.h
        ${PROJECT_SOURCE_DIR}/src/private/dsettingscontainer.cpp
    QML_FILES
        "../qml/settings/SettingsDialog.qml"
        "../qml/settings/OptionDelegate.qml"
        "../qml/settings/CheckBox.qml"
        "../qml/settings/LineEdit.qml"
        "../qml/settings/ComboBox.qml"
        "../qml/settings/NavigationTitle.qml"
        "../qml/settings/ContentTitle.qml"
        "../qml/settings/ContentBackground.qml"
    OUTPUT_DIRECTORY
        "${PROJECT_BINARY_DIR}/plugins/org/deepin/dtk/settings"
)

target_include_directories(dtkdeclarativesettingsplugin
PRIVATE
    ${PROJECT_SOURCE_DIR}/src/private
)

target_link_libraries(dtkdeclarativesettingsplugin
PRIVATE
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Quick
    Qt${QT_VERSION_MAJOR}::CorePrivate
    Qt${QT_VERSION_MAJOR}::QuickPrivate
    Qt${QT_VERSION_MAJOR}::Qml
    Qt${QT_VERSION_MAJOR}::QmlPrivate
    ${LIB_NAME}
)

