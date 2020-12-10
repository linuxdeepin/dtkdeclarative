cmake_minimum_required(VERSION 3.1)

project(%{ProjectName} LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(APP_BIN_INSTALL_DIR ${CMAKE_INSTALL_PREFIX}/bin/)
set(BIN_NAME ${CMAKE_PROJECT_NAME})
set(LIB_NAME dtkqml-${CMAKE_PROJECT_NAME})
list (APPEND RESOURCES ${CMAKE_PROJECT_NAME}.qrc)

# Set sources
set(SRC
    launcherplugin.cpp
    )

# Find the Qt5Quick library
find_package(Qt5Quick CONFIG REQUIRED)
find_package(DtkDeclarative REQUIRED)
find_package(Qt5QuickCompiler REQUIRED)

include_directories(${LIB_NAME} PUBLIC ${CMAKE_PROJECT_NAME})

qtquick_compiler_add_resources(RCC_SOURCES ${RESOURCES})

# 同时生成　exe 和　lib 两个文件
# (1)lib被exe以链接库形式使用；
# (2)lib被booster-dtkdeclarative以插件形式解析解析

# Add LIB
add_library(${LIB_NAME} SHARED
    ${SRC}
    ${RCC_SOURCES}
    )

target_link_libraries(${LIB_NAME}
    Qt5::Quick
    ${DtkDeclarative_LIBRARIES}
    )

# Add EXE
add_executable(${BIN_NAME} main.cpp)

target_link_libraries(${BIN_NAME}
    Qt5::Quick
    ${LIB_NAME}
    )

set_target_properties(${BIN_NAME} PROPERTIES INSTALL_RPATH ${DTK_QML_APP_PLUGIN_PATH})

# Add install rule
install(TARGETS ${BIN_NAME} DESTINATION ${APP_BIN_INSTALL_DIR})
install(TARGETS ${LIB_NAME} DESTINATION ${DTK_QML_APP_PLUGIN_PATH})
install(FILES %{ProjectName}.desktop DESTINATION share/applications)
