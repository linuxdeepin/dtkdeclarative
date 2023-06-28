cmake_minimum_required(VERSION 3.10)

set(DTK_VERSION "5.6.8" CACHE STRING "Define project version")
project(DtkDeclarative
    VERSION "${DTK_VERSION}"
    DESCRIPTION "DTK Declarative module"
    HOMEPAGE_URL "https://github.com/linuxdeepin/dtkdeclarative"
    LANGUAGES CXX
)
set(LIB_NAME dtkdeclarative)

if (${PROJECT_VERSION_MAJOR} STREQUAL "5")
    set(QT_DEFAULT_MAJOR_VERSION "5")
    set(QT_VERSION_MAJOR "5")
    set(EnableQt5 on)
    set(EnableQt6 off)
elseif(${PROJECT_VERSION_MAJOR} STREQUAL "6")
    set(QT_DEFAULT_MAJOR_VERSION "6")
    set(QT_VERSION_MAJOR "6")
    set(DTK_VERSION_MAJOR "6")
    set(EnableQt5 off)
    set(EnableQt6 on)
else()
    message(FATAL_ERROR "Only support DTK_VERSION is 5 or 6")
endif()

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX /usr)
endif()
include(GNUInstallDirs)
include(CMakePackageConfigHelpers)
set(BUILD_DOCS ON CACHE BOOL "Generate doxygen-based documentation")

set(LIB_INSTALL_DIR "${CMAKE_INSTALL_LIBDIR}" CACHE STRING "Library install path")
set(INCLUDE_INSTALL_DIR "${CMAKE_INSTALL_INCLUDEDIR}/dtk${PROJECT_VERSION_MAJOR}/DDeclarative" CACHE STRING "Headers install path")
set(TEMPLATE_INSTALL_DIR "share/qtcreator/templates/wizards/projects/qml-app-template" CACHE STRING "Directory to install QtCreator template")
set(CONFIG_INSTALL_DIR "${CMAKE_INSTALL_LIBDIR}/cmake/DtkDeclarative" CACHE STRING "CMake config file install directory")
set(PKGCONFIG_INSTALL_DIR "${CMAKE_INSTALL_LIBDIR}/pkgconfig" CACHE STRING "Directory to install pkg-config file")
set(MKSPECS_INSTALL_DIR "${CMAKE_INSTALL_LIBDIR}/qt${QT_VERSION_MAJOR}/mkspecs/modules" CACHE STRING "Qt pri module install directory")
set(QML_INSTALL_DIR "${CMAKE_INSTALL_LIBDIR}/qt${QT_VERSION_MAJOR}/qml" CACHE STRING "Qml plugin install directory")

set(USE_QQuickStylePluginPrivate OFF)
find_package(Qt${QT_VERSION_MAJOR}QuickControls2)
if(EnableQt5)
if(TARGET Qt::QuickControls2 AND TARGET Qt::QuickControls2Private)
    set(USE_QQuickStylePluginPrivate ON)
endif()
endif()
if(EnableQt6)
if(TARGET Qt6::QuickControls2 AND TARGET Qt6::QuickControls2Private)
    set(USE_QQuickStylePluginPrivate ON)
endif()
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -Wall -Wextra")
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,--as-needed")
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    # 加上 ASAN 检查后可能会导致 DEBUG 应用启动后退出。可以加上 ASAN_OPTIONS 环境变量来防止应用退出
    # ASAN_OPTIONS="halt_on_error=0" ASAN_OPTIONS="new_delete_type_mismatch=0"
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -g -fno-omit-frame-pointer")
    set(BUILD_TESTING ON)
else ()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Ofast")
endif ()

add_subdirectory(src)
add_subdirectory(chameleon)
add_subdirectory(qmlplugin)
add_subdirectory(examples)

if(EnableQt6)
    add_subdirectory(qt6)
endif()

if(BUILD_DOCS)
    add_subdirectory(docs)
endif()

if(BUILD_TESTING)
    add_subdirectory(tests)
    add_dependencies(unit-test dtkdeclarativeplugin)
endif()

# Install wizards template
install(DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/misc/qml-app-template/" DESTINATION "${TEMPLATE_INSTALL_DIR}")

configure_package_config_file(
    "${CMAKE_CURRENT_LIST_DIR}/misc/DtkDeclarativeConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/DtkDeclarativeConfig.cmake"
    INSTALL_DESTINATION "${CONFIG_INSTALL_DIR}"
)
write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/DtkDeclarativeConfigVersion.cmake"
    VERSION ${VERSION}
    COMPATIBILITY SameMajorVersion
)
# Install cmake config file
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/DtkDeclarativeConfig.cmake" DESTINATION "${CONFIG_INSTALL_DIR}")
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/DtkDeclarativeConfigVersion.cmake" DESTINATION "${CONFIG_INSTALL_DIR}")
# Install pkg-config file
configure_file("${PROJECT_SOURCE_DIR}/misc/dtkdeclarative.pc.in" "${CMAKE_CURRENT_BINARY_DIR}/dtkdeclarative.pc" @ONLY)
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/dtkdeclarative.pc" DESTINATION "${PKGCONFIG_INSTALL_DIR}")
# Install qmake module config file
configure_file("${CMAKE_CURRENT_LIST_DIR}/misc/qt_lib_dtkdeclarative.pri.in" "${CMAKE_CURRENT_BINARY_DIR}/qt_lib_dtkdeclarative.pri" @ONLY)
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/qt_lib_dtkdeclarative.pri" DESTINATION "${MKSPECS_INSTALL_DIR}")
