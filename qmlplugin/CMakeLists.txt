
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Qml Quick)
find_package(Dtk${DTK_VERSION_MAJOR}Core REQUIRED)
find_package(Dtk${DTK_VERSION_MAJOR}Gui REQUIRED)

set(URI "org/deepin/dtk")
set(PLUGIN_NAME dtkdeclarativeplugin)
if(EnableQt5)
    set(PLUGIN_INSTALL_DIR "${QML_INSTALL_DIR}/${URI}")
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/plugins/${URI}")


    add_library(${PLUGIN_NAME} SHARED
        ${CMAKE_CURRENT_LIST_DIR}/qmlplugin_plugin.cpp
        ${CMAKE_CURRENT_LIST_DIR}/qmlplugin_plugin.h
        ${CMAKE_CURRENT_LIST_DIR}/qmldir
    )

    set(QML_DIR "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/qmldir")
    # copy qmldir
    add_custom_command(TARGET ${PLUGIN_NAME}
       POST_BUILD
       COMMAND ${CMAKE_COMMAND} -E
       copy_if_different
       "${CMAKE_CURRENT_SOURCE_DIR}/qmldir"    #in-file
       "${QML_DIR}"                            #out-file
    )

    install(TARGETS ${PLUGIN_NAME} DESTINATION ${PLUGIN_INSTALL_DIR})
    install(FILES "${CMAKE_CURRENT_SOURCE_DIR}/qmldir" DESTINATION ${PLUGIN_INSTALL_DIR})

endif()

if(EnableQt6)
    qt_add_qml_module(${PLUGIN_NAME}
        PLUGIN_TARGET ${PLUGIN_NAME}
        URI "org.deepin.dtk"
        VERSION "${PROJECT_VERSION}"
        NO_GENERATE_PLUGIN_SOURCE
        SOURCES
            ${CMAKE_CURRENT_LIST_DIR}/qmlplugin_plugin.cpp
            ${CMAKE_CURRENT_LIST_DIR}/qmlplugin_plugin.h
        OUTPUT_DIRECTORY
            "${PROJECT_BINARY_DIR}/plugins/${URI}"
    )
endif()

target_link_libraries(${PLUGIN_NAME} PRIVATE
    Qt${QT_VERSION_MAJOR}::Qml
    Qt${QT_VERSION_MAJOR}::QmlPrivate
    Qt${QT_VERSION_MAJOR}::Quick
    Qt${QT_VERSION_MAJOR}::QuickPrivate
    Dtk${DTK_VERSION_MAJOR}::Core
    Dtk${DTK_VERSION_MAJOR}::Gui
    ${LIB_NAME}
)
