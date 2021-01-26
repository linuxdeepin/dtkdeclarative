cmake_minimum_required(VERSION 3.1)

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
find_package(Qt5QuickCompiler REQUIRED)

qtquick_compiler_add_resources(RCC_SOURCES ${RESOURCES})

# 保证 src 目录下头文件全局可见
include_directories(src)

# 同时生成 exe 和 lib 两个文件
# lib 文件有两个作用：
# (1) 开发阶段本地调试使用时，lib被exe以链接库形式使用；
# (2) 实际使用时，lib 会被安装到指定目录，lib 被 booster-dtkdeclarative 以插件形式解析

# Add LIB
add_library(${LIB_NAME} SHARED
    ${SRC}
    ${RCC_SOURCES}
    )

# 由于 LIB 程序为本项目的功能实现部分，需要用到的第三方库都需要在此处添加。
target_link_libraries(${LIB_NAME}
    Qt5::Quick
    ${DtkDeclarative_LIBRARIES}
    )

# Add EXE
add_executable(${BIN_NAME} main.cpp)

# 由于 EXE 只是作为启动程序加载 LIB 使用，一般来说，只需链接 ${LIB_NAME} 既可。
target_link_libraries(${BIN_NAME}
    Qt5::Quick
    ${LIB_NAME}
    )

set_target_properties(${BIN_NAME} PROPERTIES INSTALL_RPATH ${DTK_QML_APP_PLUGIN_PATH})

# Add install rule
install(TARGETS ${BIN_NAME} DESTINATION ${APP_BIN_INSTALL_DIR})
install(TARGETS ${LIB_NAME} DESTINATION ${DTK_QML_APP_PLUGIN_PATH})
install(FILES ${CMAKE_PROJECT_NAME}.desktop DESTINATION share/applications)
