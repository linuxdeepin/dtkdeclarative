set(LIB_NAME dtkdeclarative)
set(DTK_QML_APP_PLUGIN_PATH "${CMAKE_INSTALL_PREFIX}/${LIB_INSTALL_DIR}/${LIB_NAME}/qml-app" CACHE STRING "dtk qml app plugin path")
set(DTK_QML_APP_PLUGIN_SUBPATH "dtkdeclarative/plugins" CACHE STRING "dtk qml app plugin subpath")
set(DDECLARATIVE_TRANSLATIONS_DIR "dtk5/DDeclarative/translations" CACHE STRING "DDeclarative translations directory")
set(DDECLARATIVE_TRANSLATIONS_PATH "share/${DDECLARATIVE_TRANSLATIONS_DIR}")
set(TRANSLATIONS_INSTALL_PATH "${DDECLARATIVE_TRANSLATIONS_PATH}")

# Add secure compiler options
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fstack-protector-all")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fstack-protector-all")
set(CMAKE_EXE_LINKER_FLAGS  "-z relro -z now -z noexecstack -pie")
# For mips64
if (${CMAKE_SYSTEM_PROCESSOR} MATCHES "mips64")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -ftree-vectorize -march=loongson3a -mhard-float -mno-micromips -mno-mips16 -flax-vector-conversions -mloongson-ext2 -mloongson-mmi")
endif()

find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Quick DBus QuickControls2 LinguistTools)
if(EnableQt5)
    find_package(Qt${QT_VERSION_MAJOR}QuickCompiler)
endif()
if(EnableQt6)
    find_package(Qt${QT_DEFAULT_MAJOR_VERSION}ShaderTools)
endif()
find_package(Dtk${DTK_VERSION_MAJOR}Core REQUIRED)
find_package(Dtk${DTK_VERSION_MAJOR}Gui REQUIRED)
find_package(PkgConfig REQUIRED)

pkg_check_modules(GL REQUIRED IMPORTED_TARGET gl)

file(GLOB PUBLIC_HEADERS "./*.h")
file(GLOB_RECURSE HEADERS "./*.h")
file(GLOB_RECURSE SRCS   "./*.cpp")
file(GLOB D_HEADERS   "./D*")

file(GLOB TS_FILES "translations/*.ts")

include(src.cmake)

set_source_files_properties(${PROJECT_SOURCE_DIR}/chameleon/PixelMetric.js
    PROPERTIES QT_RESOURCE_ALIAS "qml/PixelMetric.js"
)

qt_add_translation(QM_FILES ${TS_FILES})

qt_add_qml_module(${LIB_NAME}
    PLUGIN_TARGET dtkdeclarativeplugin
    URI "org.deepin.dtk"
    VERSION "1.0"
    SHARED
    NO_GENERATE_PLUGIN_SOURCE
    NO_PLUGIN_OPTIONAL
    QML_FILES
        ${QML_DTK_CONTROLS}
        ${PROJECT_SOURCE_DIR}/chameleon/PixelMetric.js
    SOURCES
        ${DTK_SOURCES}
    dqmlregister_p.h
    OUTPUT_DIRECTORY
        "${PROJECT_BINARY_DIR}/plugins/org/deepin/dtk"
)

qt_add_resources(RESOURCES
    ${PROJECT_SOURCE_DIR}/src/dtkdeclarative_assets.qrc
    ${PROJECT_SOURCE_DIR}/src/dtkdeclarative_qml-qt6.qrc
)
target_sources(dtkdeclarative PRIVATE
    ${RESOURCES}
)

target_link_libraries(dtkdeclarativeplugin PRIVATE
    ${LIB_NAME}
)

target_sources(dtkdeclarativeplugin PRIVATE
    qmlplugin_plugin.h    qmlplugin_plugin.cpp
)

qt_add_shaders(${LIB_NAME} "_shaders_ng"
    BATCHABLE
    PRECOMPILE
    PREFIX
        "/dtk/declarative"
    FILES
        ${DTK_SHARERS_NG}
)

target_include_directories(${LIB_NAME} PRIVATE
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/src/private
)

set_target_properties(${LIB_NAME} PROPERTIES
    VERSION ${CMAKE_PROJECT_VERSION}
    SOVERSION ${CMAKE_PROJECT_VERSION_MAJOR}
    EXPORT_NAME Declarative
)

target_compile_definitions(${LIB_NAME} PRIVATE
    DTK_QML_APP_PLUGIN_PATH="${DTK_QML_APP_PLUGIN_PATH}"
    DTK_QML_APP_PLUGIN_SUBPATH="${DTK_QML_APP_PLUGIN_SUBPATH}"
    DDECLARATIVE_TRANSLATIONS_DIR="${DDECLARATIVE_TRANSLATIONS_DIR}"
)

if(USE_QQuickStylePluginPrivate)
    target_link_libraries(${LIB_NAME} PRIVATE
        Qt${QT_VERSION_MAJOR}::QuickControls2Private
    )
endif()

target_link_libraries(${LIB_NAME}
PUBLIC
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Quick
    Dtk${DTK_VERSION_MAJOR}::Core
    Dtk${DTK_VERSION_MAJOR}::Gui
    Qt${QT_VERSION_MAJOR}::QuickPrivate
PRIVATE
    Qt${QT_VERSION_MAJOR}::DBus
    PkgConfig::GL
)

target_include_directories(${LIB_NAME} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/src>
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/src/private>
)

target_include_directories(${LIB_NAME} INTERFACE
    $<INSTALL_INTERFACE:${INCLUDE_INSTALL_DIR}>
)

target_link_directories(${LIB_NAME} INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
    $<INSTALL_INTERFACE:${LIB_INSTALL_DIR}>
)

# gen dtkdeclarative_config.h
include(DtkBuildConfig)
set(options MSG) # print MODULE_NAME D_HEADERS DEST_DIR
gen_dtk_config_header(MODULE_NAME ${LIB_NAME} HEADERS ${D_HEADERS})

# Install library
install(TARGETS ${LIB_NAME} EXPORT DtkDeclarativeTargets DESTINATION "${LIB_INSTALL_DIR}")
# Install headers
install(FILES ${PUBLIC_HEADERS} ${D_HEADERS} DESTINATION "${INCLUDE_INSTALL_DIR}")
# Install export targets
install(EXPORT DtkDeclarativeTargets NAMESPACE Dtk:: FILE DtkDeclarativeTargets.cmake DESTINATION "${CONFIG_INSTALL_DIR}")
# Install translations
install(FILES ${QM_FILES} DESTINATION "${TRANSLATIONS_INSTALL_PATH}")
