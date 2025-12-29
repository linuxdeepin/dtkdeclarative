set(BIN_NAME dtk${DTK_NAME_SUFFIX}-exhibition)

find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Quick QuickControls2)
find_package(Dtk${DTK_NAME_SUFFIX}Core REQUIRED)
find_package(Dtk${DTK_NAME_SUFFIX}Gui REQUIRED)

set(CMAKE_EXE_LINKER_FLAGS  "-z relro -z now -z noexecstack -pie -Wl,--as-needed")

if (DTK5)
    set(QML_RCS ${CMAKE_CURRENT_LIST_DIR}/qml.qrc)
else()
    set(QML_RCS ${CMAKE_CURRENT_LIST_DIR}/qml-qt6.qrc)
endif()
add_executable(${BIN_NAME}
    ${CMAKE_CURRENT_LIST_DIR}/main.cpp
    ${CMAKE_CURRENT_LIST_DIR}/assets.qrc
    ${QML_RCS}
)

target_compile_options(${BIN_NAME} PRIVATE "-fpic")

target_link_libraries(${BIN_NAME} PRIVATE
    Qt${QT_VERSION_MAJOR}::Quick
    Qt${QT_VERSION_MAJOR}::QuickControls2
    Dtk${DTK_NAME_SUFFIX}::Core
    Dtk${DTK_NAME_SUFFIX}::Gui
)

set(EXAMPLE_INSTALL_DIR "${CMAKE_INSTALL_BINDIR}")
if (NOT DTK5)
    set(EXAMPLE_INSTALL_DIR "${LIB_INSTALL_DIR}/dtk${DTK_VERSION_MAJOR}/DDeclarative")
endif()

install(TARGETS ${BIN_NAME} DESTINATION ${EXAMPLE_INSTALL_DIR})
