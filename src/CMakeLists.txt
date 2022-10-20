set(LIB_NAME dtkdeclarative)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

#include(GNUInstallDirs)
if (${CMAKE_INSTALL_LIBDIR} STREQUAL "lib")
    message("redirector from [${CMAKE_INSTALL_LIBDIR}] to [${CMAKE_CURRENT_BINARY_DIR}/lib]")
    set(CMAKE_INSTALL_LIBDIR ${CMAKE_CURRENT_BINARY_DIR}/lib)
endif()

set(LIB_INSTALL_DIR ${CMAKE_INSTALL_LIBDIR} CACHE STRING "lib install path")
set(DTK_QML_APP_PLUGIN_PATH "${LIB_INSTALL_DIR}/${LIB_NAME}/qml-app" CACHE STRING "dtk qml app plugin path")
#message("lib install dir :${LIB_INSTALL_DIR}")
#message("dtk qml app plugin path :${DTK_QML_APP_PLUGIN_PATH}")
add_definitions(-DDTK_QML_APP_PLUGIN_PATH=\"${DTK_QML_APP_PLUGIN_PATH}\")

# 增加安全编译参数
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fstack-protector-all")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fstack-protector-all")
set(CMAKE_EXE_LINKER_FLAGS  "-z relro -z now -z noexecstack -pie")
# for mips64
if (${CMAKE_SYSTEM_PROCESSOR} MATCHES "mips64")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -ftree-vectorize -march=loongson3a -mhard-float -mno-micromips -mno-mips16 -flax-vector-conversions -mloongson-ext2 -mloongson-mmi")
endif()

find_package(Qt5 REQUIRED COMPONENTS Core Quick QuickCompiler DBus QuickControls2)
find_package(DtkCore REQUIRED)
find_package(DtkGui REQUIRED)
if(USE_QQuickStylePluginPrivate)
  set(include_DIRS 
    ${Qt5Core_INCLUDE_DIRS}
    ${Qt5Quick_INCLUDE_DIRS}
    ${Qt5DBus_INCLUDE_DIRS}
    ${Qt5Quick_PRIVATE_INCLUDE_DIRS}
    ${DtkCore_INCLUDE_DIRS}
    ${DtkGui_INCLUDE_DIRS}
  )
else()
  set(include_DIRS 
    ${Qt5Core_INCLUDE_DIRS}
    ${Qt5Quick_INCLUDE_DIRS}
    ${Qt5DBus_INCLUDE_DIRS}
    ${Qt5Quick_PRIVATE_INCLUDE_DIRS}
    ${Qt5QuickControls2_PRIVATE_INCLUDE_DIRS}
    ${DtkCore_INCLUDE_DIRS}
    ${DtkGui_INCLUDE_DIRS}
  )
endif()

file(GLOB_RECURSE HEADERS "./*.h")
file(GLOB_RECURSE SRCS   "./*.cpp")
file(GLOB D_HEADERS   "./D*")
file(GLOB QML_RCS ${CMAKE_CURRENT_LIST_DIR}/dtkdeclarative_qml.qrc)
file(GLOB ASSETS_RCS ${CMAKE_CURRENT_LIST_DIR}/dtkdeclarative_assets.qrc)
# quick compiler gen qmlc
qtquick_compiler_add_resources(RESOURCES ${QML_RCS})

add_library(${LIB_NAME} SHARED
    ${SRCS}
    ${HEADERS} 
    ${D_HEADERS}
    ${RESOURCES}
    ${ASSETS_RCS}
)

set_target_properties(${LIB_NAME} PROPERTIES
  VERSION ${CMAKE_PROJECT_VERSION}
  SOVERSION ${CMAKE_PROJECT_VERSION_MAJOR}
)

target_include_directories( ${LIB_NAME} PUBLIC
  ${include_DIRS}
)

target_link_libraries(${LIB_NAME} PRIVATE
    Qt5::Core
    Qt5::Quick 
    Qt5::DBus
    ${DtkCore_LIBRARIES}
    ${DtkGui_LIBRARIES}
)

# gen dtkdeclarative_config.h
include(../cmake/DtkBuildConfig.cmake)
set(options MSG) # print MODULE_NAME D_HEADERS DEST_DIR
gen_dtk_config_header(MODULE_NAME ${LIB_NAME} HEADERS ${D_HEADERS})

# install so
install(TARGETS ${LIB_NAME} DESTINATION "${CMAKE_INSTALL_LIBDIR}")
# install headers
install(FILES ${HEADERS} ${D_HEADERS} DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/libdtk-${PROJECT_VERSION}/DDeclarative")
# install wizards templates
file(GLOB_RECURSE QML_TEMPLATE ${CMAKE_CURRENT_LIST_DIR}/../misc/qml-app-template/*)
install(FILES ${QML_TEMPLATE} DESTINATION "/usr/share/qtcreator/templates/wizards/projects")
# install cmake config file
configure_file(${CMAKE_CURRENT_LIST_DIR}/../misc/DtkDeclarativeConfig.cmake.in DtkDeclarativeConfig.cmake @ONLY)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/DtkDeclarativeConfig.cmake DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/DtkDeclarative")
# install pkg-config file
configure_file(${CMAKE_CURRENT_LIST_DIR}/../misc/dtkdeclarative.pc.in dtkdeclarative.pc @ONLY)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/dtkdeclarative.pc DESTINATION "${CMAKE_INSTALL_LIBDIR}/pkgconfig/")
# install qmake module config file
configure_file(${CMAKE_CURRENT_LIST_DIR}/../misc/qt_lib_dtkdeclarative.pri.in qt_lib_dtkdeclarative.pri @ONLY)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/qt_lib_dtkdeclarative.pri DESTINATION "${CMAKE_INSTALL_LIBDIR}/qt5/mkspecs/modules")
