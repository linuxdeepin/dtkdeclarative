set(BIN_NAME qml-inspect)

find_package(Qt5 COMPONENTS Quick QuickControls2)
find_package(Dtk REQUIRED COMPONENTS Gui)

add_executable(${BIN_NAME}
    ${CMAKE_CURRENT_LIST_DIR}/main.cpp 
    ${CMAKE_CURRENT_LIST_DIR}/qml.qrc
    ${CMAKE_CURRENT_LIST_DIR}/dciicons.qrc
)

target_link_libraries(${BIN_NAME} PUBLIC
    Qt5::Quick 
    Qt5::QuickControls2
    Dtk::Gui
    ${LIB_NAME}
)
