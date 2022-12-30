set(URI "org/deepin/dtk")
set(PLUGIN_INSTALL_DIR "${CMAKE_INSTALL_LIBDIR}/qt5/qml/${URI}")
set(PLUGIN_NAME dtkdeclarativeplugin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/plugins/${URI}")


add_library(${PLUGIN_NAME} SHARED
    ${CMAKE_CURRENT_LIST_DIR}/qmlplugin_plugin.cpp
    ${CMAKE_CURRENT_LIST_DIR}/qmlplugin_plugin.h
    ${CMAKE_CURRENT_LIST_DIR}/qmldir
)

find_package(Qt5 REQUIRED COMPONENTS Core Qml Quick)
find_package(Dtk REQUIRED COMPONENTS Core Gui)

target_link_libraries(${PLUGIN_NAME} PRIVATE
    Qt5::Qml
    Qt5::QmlPrivate
    Qt5::Quick
    Qt5::QuickPrivate
    Dtk::Core
    Dtk::Gui
    ${LIB_NAME}
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
