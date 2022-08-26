set(BIN_NAME ut-test)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
# dbus
set(CMAKE_CXX_FLAGS "-fno-access-control -ldl")
add_definitions(-DPREFIX="${CMAKE_PREFIX_PATH}")

find_package(DtkCore REQUIRED)
find_package(DtkGui REQUIRED)
find_package(GTest REQUIRED)
find_package(Qt5 COMPONENTS Core Gui Quick Qml Test REQUIRED)

file(GLOB TEST_HEADER  ut_.*h)
file(GLOB TEST_SOURCE  *.cpp)

set(test_SRC 
  ${TEST_HEADER}
  ${TEST_SOURCE}
)

# 保证 preloadplugin 目录下头文件全局可见
include_directories(../src)

add_executable(${BIN_NAME}
    ${test_SRC}
    ./data.qrc
)

target_link_directories(${BIN_NAME} PRIVATE
  ${CMAKE_CURRENT_BINARY_DIR}/../src
)

target_link_libraries(
  ${BIN_NAME} PRIVATE
    Qt5::Gui 
    Qt5::Core 
    Qt5::Test
    Qt5::Qml
    Qt5::Quick
    ${DtkCore_LIBRARIES}
    ${DtkGui_LIBRARIES}
    ${GTEST_LIBRARIES}
    dtkdeclarative
    -lpthread
)

target_include_directories( ${BIN_NAME} PUBLIC
    ${Qt5Core_PRIVATE_INCLUDE_DIRS}
)

