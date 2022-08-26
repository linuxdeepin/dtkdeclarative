set(pluginName dtkdeclarativeplugin)

set(CMAKE_AUTOMOC ON)
set(uri_path "org/deepin/dtk")
message("uri_path in ${pluginName} : ${uri_path}")

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/../chameleon/imports/${uri_path}")

add_library(${pluginName} SHARED
    ${CMAKE_CURRENT_LIST_DIR}/qmlplugin_plugin.cpp
    ${CMAKE_CURRENT_LIST_DIR}/qmlplugin_plugin.h
    ${CMAKE_CURRENT_LIST_DIR}/qmldir
)

find_package(Qt5 REQUIRED COMPONENTS Core Qml Quick)
find_package(DtkCore REQUIRED)
find_package(DtkGui REQUIRED)

target_include_directories(${pluginName} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/../src
    ${Qt5Qml_INCLUDE_DIRS}
    ${Qt5Qml_PRIVATE_INCLUDE_DIRS}
    ${Qt5Quick_PRIVATE_INCLUDE_DIRS}
    ${DTKGUI_INCLUDE_DIR}
    ${DTKCORE_INCLUDE_DIR}
)

target_link_directories(${pluginName} PUBLIC
    ${CMAKE_CURRENT_BINARY_DIR}/../src
)

target_link_libraries(${pluginName} PUBLIC
    Qt5::Qml
    Qt5::Quick
    ${DtkCore_LIBRARIES}
    ${DtkGui_LIBRARIES}
    dtkdeclarative
)

set(QML_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../chameleon/imports/${uri_path}/qmldir")
# copy qmldir
add_custom_command(TARGET ${pluginName}
   POST_BUILD
   COMMAND ${CMAKE_COMMAND} -E
   copy_if_different
   "${CMAKE_CURRENT_SOURCE_DIR}/qmldir"    #in-file
   "${QML_DIR}"                            #out-file
)
include(GNUInstallDirs)
set(DEST_DIR "${CMAKE_INSTALL_LIBDIR}/qt5/qml/${uri_path}")
install(TARGETS ${pluginName} DESTINATION ${DEST_DIR})
install(FILES ${QML_DIR} DESTINATION ${DEST_DIR})
