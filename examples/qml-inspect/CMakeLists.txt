set(BIN_NAME qml-inspect)

find_package(Qt${QT_DEFAULT_MAJOR_VERSION} COMPONENTS Quick QuickControls2)
find_package(Dtk${DTK_DEFAULT_MAJOR_VERSION}Core REQUIRED)
find_package(Dtk${DTK_DEFAULT_MAJOR_VERSION}Gui REQUIRED)

add_executable(${BIN_NAME}
    ${CMAKE_CURRENT_LIST_DIR}/main.cpp 
    ${CMAKE_CURRENT_LIST_DIR}/qml.qrc
    ${CMAKE_CURRENT_LIST_DIR}/dciicons.qrc
)

target_link_libraries(${BIN_NAME} PUBLIC
    Qt${QT_VERSION_MAJOR}::Quick 
    Qt${QT_VERSION_MAJOR}::QuickControls2
    Dtk${DTK_VERSION_MAJOR}::Gui
    ${LIB_NAME}
)
