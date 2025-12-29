cmake_minimum_required(VERSION 3.10)

file(READ "${CMAKE_CURRENT_SOURCE_DIR}/VERSION" FILE_VERSION)
string(STRIP "${FILE_VERSION}" FILE_VERSION)

project(DtkDeclarative
    VERSION ${FILE_VERSION}
    DESCRIPTION "DTK Declarative module"
    HOMEPAGE_URL "https://github.com/linuxdeepin/dtkdeclarative"
    LANGUAGES CXX
)

option(DTK5 "Build DTK5." ON)
if(DTK5)
    set(DTK_VERSION_MAJOR "5")
    set(DTK_NAME_SUFFIX "")
else()
    set(DTK_VERSION_MAJOR "6")
    set(DTK_NAME_SUFFIX "6")
endif()


set(DTK_VERSION_MINOR ${PROJECT_VERSION_MINOR})
set(DTK_VERSION_PATCH ${PROJECT_VERSION_PATCH})
set(DTK_VERSION "${DTK_VERSION_MAJOR}.${DTK_VERSION_MINOR}.${DTK_VERSION_PATCH}")
set(QT_VERSION_MAJOR ${DTK_VERSION_MAJOR})

set(ENABLE_COV OFF CACHE BOOL "Generate coverage info")
set(LIB_NAME dtk${DTK_NAME_SUFFIX}declarative)

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

include(DtkBuildConfig)
set(BUILD_DOCS ON CACHE BOOL "Generate doxygen-based documentation")

set(LIB_INSTALL_DIR "${CMAKE_INSTALL_LIBDIR}" CACHE STRING "Library install path")
set(INCLUDE_INSTALL_DIR "${CMAKE_INSTALL_INCLUDEDIR}/dtk${DTK_VERSION_MAJOR}/DDeclarative" CACHE STRING "Headers install path")
set(TEMPLATE_INSTALL_DIR "${CMAKE_INSTALL_PREFIX}/share/qtcreator/templates/wizards/projects/qml${DTK_VERSION_MAJOR}-app-template" CACHE STRING "Directory to install QtCreator template")
set(CONFIG_INSTALL_DIR "${CMAKE_INSTALL_LIBDIR}/cmake/Dtk${DTK_NAME_SUFFIX}Declarative" CACHE STRING "CMake config file install directory")
set(PKGCONFIG_INSTALL_DIR "${CMAKE_INSTALL_LIBDIR}/pkgconfig" CACHE STRING "Directory to install pkg-config file")
set(MKSPECS_INSTALL_DIR "${CMAKE_INSTALL_LIBDIR}/qt${QT_VERSION_MAJOR}/mkspecs/modules" CACHE STRING "Qt pri module install directory")
set(QML_INSTALL_DIR "${CMAKE_INSTALL_LIBDIR}/qt${QT_VERSION_MAJOR}/qml" CACHE STRING "Qml plugin install directory")

set(USE_QQuickStylePluginPrivate OFF)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core QuickControls2)

if (${QT_VERSION_MAJOR} STREQUAL "6")
    if (${Qt6QuickControls2_VERSION} VERSION_GREATER_EQUAL "6.10.0")
        set(QT_NO_PRIVATE_MODULE_WARNING ON)
        find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS CorePrivate QuickControls2Private)
    endif()
endif()

if(DTK5)
if(TARGET Qt::QuickControls2 AND TARGET Qt::QuickControls2Private)
    set(USE_QQuickStylePluginPrivate ON)
endif()
else()
if(TARGET Qt6::QuickControls2 AND TARGET Qt6::QuickControls2Private)
    set(USE_QQuickStylePluginPrivate ON)
endif()
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -Wextra")
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    # 加上 ASAN 检查后可能会导致 DEBUG 应用启动后退出。可以加上 ASAN_OPTIONS 环境变量来防止应用退出
    # ASAN_OPTIONS="halt_on_error=0" ASAN_OPTIONS="new_delete_type_mismatch=0"
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -g -fno-omit-frame-pointer")
    set(BUILD_TESTING ON)
else ()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Ofast")
endif ()

set(DDECLARATIVE_TRANSLATIONS_DIR "dtk${DTK_VERSION_MAJOR}/DDeclarative/translations" CACHE STRING "DDeclarative translations directory")
set(DDECLARATIVE_TRANSLATIONS_PATH "share/${DDECLARATIVE_TRANSLATIONS_DIR}")
set(TRANSLATIONS_INSTALL_PATH "${DDECLARATIVE_TRANSLATIONS_PATH}")

set(URI "org.deepin.dtk")
string(REPLACE "." "/" URI_PATH ${URI})
set(PLUGIN_NAME dtkdeclarativeplugin)
set(STYLE_PLUGIN_NAME qtquickcontrolschameleonstyleplugin)
set(PLUGIN_OUTPUT_DIR ${PROJECT_BINARY_DIR}/plugins)

if(DTK5)
    add_subdirectory(src)
    add_subdirectory(qmlplugin)
else()
    add_subdirectory(qt6)
endif()
add_subdirectory(chameleon)
add_subdirectory(examples)

if(BUILD_DOCS)
    add_subdirectory(docs)
endif()

if(BUILD_TESTING)
    enable_testing()
    add_subdirectory(tests)
    add_dependencies(unit-test ${PLUGIN_NAME} ${STYLE_PLUGIN_NAME})
    if(NOT DTK5)
        add_dependencies(unit-test dtkdeclarativeprivatesplugin dtkdeclarativesettingsplugin)
    endif()
endif()

# Install wizards template
set(QML_TEMPLATE_QTVERSION_INDEX 0)
if(DTK5)
    set(QML_TEMPLATE_QTVERSION_INDEX 3)
endif()
configure_package_config_file(
    "${CMAKE_CURRENT_LIST_DIR}/misc/qml-app-template/wizard.json.in"
    "${CMAKE_CURRENT_BINARY_DIR}/wizard.json"
    INSTALL_DESTINATION "${TEMPLATE_INSTALL_DIR}"
)
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/wizard.json" DESTINATION "${TEMPLATE_INSTALL_DIR}")
install(DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/misc/qml-app-template/" DESTINATION "${TEMPLATE_INSTALL_DIR}" FILES_MATCHING PATTERN "*" PATTERN "*.in" EXCLUDE)

configure_package_config_file(
    "${CMAKE_CURRENT_LIST_DIR}/misc/DtkDeclarativeConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/Dtk${DTK_NAME_SUFFIX}DeclarativeConfig.cmake"
    INSTALL_DESTINATION "${CONFIG_INSTALL_DIR}"
)
write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/Dtk${DTK_NAME_SUFFIX}DeclarativeConfigVersion.cmake"
    VERSION ${DTK_VERSION}
    COMPATIBILITY SameMajorVersion
)
# Install cmake config file
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/Dtk${DTK_NAME_SUFFIX}DeclarativeConfig.cmake" DESTINATION "${CONFIG_INSTALL_DIR}")
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/Dtk${DTK_NAME_SUFFIX}DeclarativeConfigVersion.cmake" DESTINATION "${CONFIG_INSTALL_DIR}")
# Install pkg-config file
configure_file("${PROJECT_SOURCE_DIR}/misc/dtkdeclarative.pc.in" "${CMAKE_CURRENT_BINARY_DIR}/dtk${DTK_NAME_SUFFIX}declarative.pc" @ONLY)
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/dtk${DTK_NAME_SUFFIX}declarative.pc" DESTINATION "${PKGCONFIG_INSTALL_DIR}")
# Install qmake module config file
configure_file("${CMAKE_CURRENT_LIST_DIR}/misc/qt_lib_dtkdeclarative.pri.in" "${CMAKE_CURRENT_BINARY_DIR}/qt_lib_dtkdeclarative.pri" @ONLY)
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/qt_lib_dtkdeclarative.pri" DESTINATION "${MKSPECS_INSTALL_DIR}")
