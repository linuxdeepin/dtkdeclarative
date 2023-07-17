set(BIN_NAME unit-test)

find_package(Dtk${DTK_VERSION_MAJOR}Core REQUIRED)
find_package(Dtk${DTK_VERSION_MAJOR}Gui REQUIRED)
find_package(GTest REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Gui Quick QuickControls2 Qml Test)

file(GLOB TEST_SOURCES
    ut_colorselector.cpp
    ut_dconfigwrapper.cpp
    ut_dqmlglobalobject.cpp
    ut_dapploader.cpp
)
if (EnableDtk5)
    list(APPEND TEST_SOURCES
        ut_dplatformthemeproxy.cpp
    )
endif()

include(${PROJECT_SOURCE_DIR}/src/src.cmake)

set(CHAMELEON_PATH "${PROJECT_BINARY_DIR}/plugins")
add_definitions(
    -DCHAMELEON_PATH="${CHAMELEON_PATH}"
)

add_executable(${BIN_NAME}
    main.cpp ${TEST_SOURCES} ${HEADERS} ${SRCS}
    ${CMAKE_CURRENT_LIST_DIR}/data.qrc
)

target_compile_definitions(${BIN_NAME} PRIVATE
    BIN_NAME="${BIN_NAME}"
)

target_compile_options(${BIN_NAME} PRIVATE
    "-fno-access-control"
)
if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    target_compile_options(${BIN_NAME} PRIVATE -fprofile-instr-generate -ftest-coverage)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_compile_options(${BIN_NAME} PRIVATE -fprofile-arcs -ftest-coverage)
endif()

add_definitions(-DQML_PLUGIN_PATH="${PROJECT_BINARY_DIR}/plugins/")

target_compile_definitions(${BIN_NAME} PRIVATE
    DTK_QML_APP_PLUGIN_PATH="${DTK_QML_APP_PLUGIN_PATH}"
    DTK_QML_APP_PLUGIN_SUBPATH="${DTK_QML_APP_PLUGIN_SUBPATH}"
    DDECLARATIVE_TRANSLATIONS_DIR="${DDECLARATIVE_TRANSLATIONS_DIR}"
)

find_package(PkgConfig REQUIRED)

pkg_check_modules(GL REQUIRED IMPORTED_TARGET gl)

target_link_libraries(${BIN_NAME} PRIVATE
    Qt${QT_VERSION_MAJOR}::Gui
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::CorePrivate
    Qt${QT_VERSION_MAJOR}::Test
    Qt${QT_VERSION_MAJOR}::Qml
    Qt${QT_VERSION_MAJOR}::Quick
    Qt${QT_VERSION_MAJOR}::QuickControls2
    Dtk${DTK_VERSION_MAJOR}::Core
    Dtk${DTK_VERSION_MAJOR}::Gui
    Qt${QT_VERSION_MAJOR}::QuickPrivate
    GTest::GTest
    PkgConfig::GL
    pthread
    dl
    m
    gcov
)

target_include_directories(${BIN_NAME} PRIVATE
    ${PROJECT_SOURCE_DIR}/src
)
