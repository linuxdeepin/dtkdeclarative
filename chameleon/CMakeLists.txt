set(PLUGIN_NAME qtquickcontrols2chameleonstyleplugin)
set(TARGETPATH "QtQuick/Controls.2/Chameleon")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/plugins/Chameleon")
set(PLUGIN_INSTALL_DIR "${QML_INSTALL_DIR}/${TARGETPATH}")

find_package(DtkGui REQUIRED)
find_package(Qt5 COMPONENTS
    Qml
    QuickCompiler
    Quick
    QuickControls2 REQUIRED
)

file(GLOB QML_FILES "imports/Chameleon/*.qml")
file(GLOB JS_FILES "imports/Chameleon/*.js")
file(GLOB SRC_FILES "src/*.h" "src/*.cpp")

qtquick_compiler_add_resources(RESOURCES ${CMAKE_CURRENT_LIST_DIR}/imports/Chameleon/qml.qrc)

add_library(${PLUGIN_NAME} SHARED
    ${SRC_FILES}
    ${RESOURCES} #   imports/Chameleon/qml.qrc
    ${QML_DIR}
)

target_link_libraries(${PLUGIN_NAME} PRIVATE
    Qt5::Qml
    Qt5::Quick
    Qt5::QuickControls2
)

if(USE_QQuickStylePluginPrivate)
    target_link_libraries(${PLUGIN_NAME} PRIVATE
        Qt5::QuickControls2Private
    )
endif()

find_program(COMPILER 
    NAMES qmlcachegen
    REQUIRED
)

set(QMLC_FILES)
foreach(QML_FILE ${QML_FILES})
    get_filename_component(EXTENSION ${QML_FILE} EXT)
    if(EXTENSION STREQUAL ".qml" OR EXTENSION STREQUAL ".js" OR EXTENSION STREQUAL ".ui.qml")
        set(OUTPUT_FILE "${QML_FILE}c")
        execute_process(COMMAND ${COMPILER} ${QML_FILE} -o ${OUTPUT_FILE})
        list(APPEND QMLC_FILES ${OUTPUT_FILE})
    endif()
endforeach()

set(QML_DIR "${CMAKE_CURRENT_BINARY_DIR}/qmldir")
configure_file("${CMAKE_CURRENT_LIST_DIR}/imports/Chameleon/qmldir.in" "${QML_DIR}" @ONLY)
set(OUT_QML_DIR "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/qmldir")
# copy qmldir
add_custom_command(TARGET ${PLUGIN_NAME}
   POST_BUILD
   COMMAND ${CMAKE_COMMAND} -E
   copy_if_different
   "${QML_DIR}"    #in-file
   "${OUT_QML_DIR}"                            #out-file
)

install(TARGETS ${PLUGIN_NAME} DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES
    ${QML_DIR}
    ${JS_FILES}
    ${QML_FILES}
    ${QMLC_FILES}
    DESTINATION ${PLUGIN_INSTALL_DIR}
)
