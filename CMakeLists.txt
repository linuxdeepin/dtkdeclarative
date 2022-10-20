cmake_minimum_required (VERSION 3.10)

set (VERSION "1.0.10" CACHE STRING "define project version")

project (DtkDeclarative
    VERSION "${VERSION}"
    DESCRIPTION "DTK Declarative module"
    HOMEPAGE_URL "https://github.com/linuxdeepin/dtkdeclarative"
    LANGUAGES CXX
)

include(GNUInstallDirs)

set (BUILD_DOCS ON CACHE BOOL "Generate doxygen-based documentation")
set (BUILD_TESTING ON CACHE BOOL "Build unit tests")

message("
    BUILD_DOCS:   ${BUILD_DOCS} \n
    BUILD_TESTING:${BUILD_TESTING} \n"
)

if (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX /usr)
endif ()

set (USE_QQuickStylePluginPrivate OFF)
find_package(Qt5QuickControls2)
try_compile(QQuickStylePluginPrivateTest
              ${CMAKE_CURRENT_BINARY_DIR}/try_compile
            SOURCES
              ${CMAKE_CURRENT_SOURCE_DIR}/try_compile/try_compile.cpp
            LINK_LIBRARIES
              Qt5::QuickControls2
            CMAKE_FLAGS
              "-DINCLUDE_DIRECTORIES=${Qt5QuickControls2_PRIVATE_INCLUDE_DIRS}"
            OUTPUT_VARIABLE
              result
)
if(QQuickStylePluginPrivateTest)
  message("=======================================================")
  message("    You have Qt5QuickControls2_PRIVATE_INCLUDE_DIRS")
  message("=======================================================")
  add_definitions(-DUSE_QQuickStylePlugin)
  set (USE_QQuickStylePluginPrivate ON)
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -Wall -Wextra")
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,--as-needed")
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    # 加上 ASAN 检查后可能会导致 DEBUG 应用启动后退出。可以加上 ASAN_OPTIONS 环境变量来防止应用退出
    # ASAN_OPTIONS="halt_on_error=0" ASAN_OPTIONS="new_delete_type_mismatch=0"
    # set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -g -fsanitize=address -fno-omit-frame-pointer")
else ()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Ofast")
endif ()

set(uri_path "org/deepin/dtk")

add_subdirectory(src)
add_subdirectory(chameleon)
add_subdirectory(qmlplugin)
add_subdirectory(examples)

if (BUILD_DOCS)
    add_subdirectory(docs)
endif ()

if (BUILD_TESTING)
    add_subdirectory(tests)
endif ()
