#####################################################################
## dtkdeclarativesettingsplugin Plugin:
#####################################################################

qt_add_qml_module(dtkdeclarativesettingsplugin
    PLUGIN_TARGET dtkdeclarativesettingsplugin
    URI "${URI}.settings"
    VERSION "1.0"
    SOURCES
        ${PROJECT_SOURCE_DIR}/src/private/dsettingscontainer_p.h
        ${PROJECT_SOURCE_DIR}/src/private/dsettingscontainer.cpp
    QML_FILES
        "SettingsDialog.qml"
        "OptionDelegate.qml"
        "CheckBox.qml"
        "LineEdit.qml"
        "ComboBox.qml"
        "NavigationTitle.qml"
        "ContentTitle.qml"
        "ContentBackground.qml"
    OUTPUT_DIRECTORY
        "${PLUGIN_OUTPUT_DIR}/${URI_PATH}/settings"
)

dtk_extend_target(dtkdeclarativesettingsplugin EnableCov ${ENABLE_COV})

if("${QT_VERSION_MAJOR}" STREQUAL "6")
    if (${Qt6Core_VERSION} VERSION_GREATER_EQUAL "6.10.0")
        find_package(Qt6 REQUIRED COMPONENTS CorePrivate QuickPrivate QmlPrivate)
    endif()
endif()

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

# 禁用此插件的 RPATH/RUNPATH，因为依赖库在标准系统路径中
set_target_properties(dtkdeclarativesettingsplugin PROPERTIES
    INSTALL_RPATH ""
    BUILD_WITH_INSTALL_RPATH TRUE
)

install(TARGETS dtkdeclarativesettingsplugin DESTINATION "${QML_INSTALL_DIR}/${URI_PATH}/settings")
install(DIRECTORY "${PLUGIN_OUTPUT_DIR}/${URI_PATH}/settings/" DESTINATION "${QML_INSTALL_DIR}/${URI_PATH}/settings")
