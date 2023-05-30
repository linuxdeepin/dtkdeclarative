set(BIN_NAME dtk-exhibition)

find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Quick QuickControls2)
find_package(Dtk${DTK_VERSION_MAJOR}Core REQUIRED)
find_package(Dtk${DTK_VERSION_MAJOR}Gui REQUIRED)

set(CMAKE_EXE_LINKER_FLAGS  "-z relro -z now -z noexecstack -pie -Wl,--as-needed")

add_executable(${BIN_NAME}
    ${CMAKE_CURRENT_LIST_DIR}/main.cpp 
    ${CMAKE_CURRENT_LIST_DIR}/assets.qrc
    ${CMAKE_CURRENT_LIST_DIR}/qml.qrc
)

target_compile_options(${BIN_NAME} PRIVATE "-fpic")

target_link_libraries(${BIN_NAME} PRIVATE
    Qt${QT_VERSION_MAJOR}::Quick 
    Qt${QT_VERSION_MAJOR}::QuickControls2
    Dtk${DTK_VERSION_MAJOR}::Core
    Dtk${DTK_VERSION_MAJOR}::Gui
    ${LIB_NAME}
)

install(TARGETS ${BIN_NAME} DESTINATION "${CMAKE_INSTALL_BINDIR}")
install(FILES ${CMAKE_CURRENT_LIST_DIR}/dtk-exhibition.desktop DESTINATION "${CMAKE_INSTALL_DATADIR}/applications")
