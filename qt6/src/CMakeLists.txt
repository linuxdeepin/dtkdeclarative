set(LIB_NAME dtkdeclarative6)
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
file(GLOB ASSETS_RCS ${CMAKE_CURRENT_LIST_DIR}/dtkdeclarative_assets.qrc)
file(GLOB TS_FILES "translations/*.ts")

set(DTK_SOURCES
    ${PROJECT_SOURCE_DIR}/src/private/dblitframebuffernode.cpp    ${PROJECT_SOURCE_DIR}/src/private/dblitframebuffernode_p.h
    ${PROJECT_SOURCE_DIR}/src/private/dblurimagenode.cpp    ${PROJECT_SOURCE_DIR}/src/private/dblurimagenode_p.h
    ${PROJECT_SOURCE_DIR}/src/private/dcoloroverlaynode.cpp    ${PROJECT_SOURCE_DIR}/src/private/dcoloroverlaynode.h
    ${PROJECT_SOURCE_DIR}/src/private/dconfigwrapper_p.h ${PROJECT_SOURCE_DIR}/src/private/dconfigwrapper.cpp
    ${PROJECT_SOURCE_DIR}/src/private/dhandlecontextmenuwindow_p.h    ${PROJECT_SOURCE_DIR}/src/private/dhandlecontextmenuwindow.cpp
    ${PROJECT_SOURCE_DIR}/src/private/dmaskeffectnode_p.h    ${PROJECT_SOURCE_DIR}/src/private/dmaskeffectnode.cpp
    ${PROJECT_SOURCE_DIR}/src/private/dmessagemanager_p.h    ${PROJECT_SOURCE_DIR}/src/private/dmessagemanager.cpp
    ${PROJECT_SOURCE_DIR}/src/private/dobjectmodelproxy_p.h    ${PROJECT_SOURCE_DIR}/src/private/dobjectmodelproxy.cpp
    ${PROJECT_SOURCE_DIR}/src/private/dopacitymasknode.cpp    ${PROJECT_SOURCE_DIR}/src/private/dopacitymasknode.h
    ${PROJECT_SOURCE_DIR}/src/private/dpopupwindowhandle_p.h    ${PROJECT_SOURCE_DIR}/src/private/dpopupwindowhandle.cpp
    ${PROJECT_SOURCE_DIR}/src/private/dqmlglobalobject_p_p.h    ${PROJECT_SOURCE_DIR}/src/private/dqmlglobalobject_p.h    ${PROJECT_SOURCE_DIR}/src/private/dqmlglobalobject.cpp
    ${PROJECT_SOURCE_DIR}/src/private/dquickapploaderitem_p.h    ${PROJECT_SOURCE_DIR}/src/private/dquickapploaderitem.cpp
    ${PROJECT_SOURCE_DIR}/src/private/dquickarrowboxpath_p.h    ${PROJECT_SOURCE_DIR}/src/private/dquickarrowboxpath.cpp
    ${PROJECT_SOURCE_DIR}/src/private/dquickbehindwindowblur_p_p.h    ${PROJECT_SOURCE_DIR}/src/private/dquickbehindwindowblur_p.h
    ${PROJECT_SOURCE_DIR}/src/private/dquickbehindwindowblur.cpp    ${PROJECT_SOURCE_DIR}/src/private/dquickbusyindicator_p.h    ${PROJECT_SOURCE_DIR}/src/private/dquickbusyindicator.cpp
    ${PROJECT_SOURCE_DIR}/src/private/dquickcoloroverlay_p.h    ${PROJECT_SOURCE_DIR}/src/private/dquickcoloroverlay.cpp
    ${PROJECT_SOURCE_DIR}/src/private/dquickcontrolpalette_p.h    ${PROJECT_SOURCE_DIR}/src/private/dquickcontrolpalette.cpp
    ${PROJECT_SOURCE_DIR}/src/private/dquickdciicon_p.h    ${PROJECT_SOURCE_DIR}/src/private/dquickdciicon.cpp
    ${PROJECT_SOURCE_DIR}/src/private/dquickdciiconimage_p_p.h    ${PROJECT_SOURCE_DIR}/src/private/dquickdciiconimage_p.h    ${PROJECT_SOURCE_DIR}/src/private/dquickdciiconimage.cpp
    ${PROJECT_SOURCE_DIR}/src/private/dquickglow_p_p.h    ${PROJECT_SOURCE_DIR}/src/private/dquickglow_p.h    ${PROJECT_SOURCE_DIR}/src/private/dquickglow.cpp
    ${PROJECT_SOURCE_DIR}/src/private/dquickiconimage_p_p.h    ${PROJECT_SOURCE_DIR}/src/private/dquickiconimage_p.h    ${PROJECT_SOURCE_DIR}/src/private/dquickiconimage.cpp
    ${PROJECT_SOURCE_DIR}/src/private/dquickiconlabel_p_p.h    ${PROJECT_SOURCE_DIR}/src/private/dquickiconlabel_p.h    ${PROJECT_SOURCE_DIR}/src/private/dquickiconlabel.cpp
    ${PROJECT_SOURCE_DIR}/src/private/dquickimageprovider_p.h    ${PROJECT_SOURCE_DIR}/src/private/dquickimageprovider.cpp
    ${PROJECT_SOURCE_DIR}/src/private/dquickinwindowblur_p.h    ${PROJECT_SOURCE_DIR}/src/private/dquickinwindowblur.cpp
    ${PROJECT_SOURCE_DIR}/src/private/dquickitemviewport_p.h    ${PROJECT_SOURCE_DIR}/src/private/dquickkeylistener_p.h    ${PROJECT_SOURCE_DIR}/src/private/dquickkeylistener.cpp
    ${PROJECT_SOURCE_DIR}/src/private/dquickopacitymask_p_p.h    ${PROJECT_SOURCE_DIR}/src/private/dquickopacitymask_p.h    ${PROJECT_SOURCE_DIR}/src/private/dquickopacitymask.cpp
    ${PROJECT_SOURCE_DIR}/src/private/dquickrectangle_p_p.h    ${PROJECT_SOURCE_DIR}/src/private/dquickrectangle_p.h    ${PROJECT_SOURCE_DIR}/src/private/dquickrectangle.cpp
    ${PROJECT_SOURCE_DIR}/src/private/dquickwaterprogressattribute_p.h    ${PROJECT_SOURCE_DIR}/src/private/dquickwaterprogressattribute.cpp
    ${PROJECT_SOURCE_DIR}/src/private/drectanglenode_p.h    ${PROJECT_SOURCE_DIR}/src/private/drectanglenode.cpp
    ${PROJECT_SOURCE_DIR}/src/private/dsettingscontainer_p.h    ${PROJECT_SOURCE_DIR}/src/private/dsettingscontainer.cpp
    ${PROJECT_SOURCE_DIR}/src/private/dshadownode_p.cpp    ${PROJECT_SOURCE_DIR}/src/private/dshadownode_p.h
    ${PROJECT_SOURCE_DIR}/src/private/dsoftwareeffectrendernode.cpp    ${PROJECT_SOURCE_DIR}/src/private/dsoftwareeffectrendernode.h
    ${PROJECT_SOURCE_DIR}/src/private/dsoftwareroundedimagenode_p.h    ${PROJECT_SOURCE_DIR}/src/private/dsoftwareroundedimagenode.cpp
    ${PROJECT_SOURCE_DIR}/src/private/dapploader_p.h    ${PROJECT_SOURCE_DIR}/src/dapploader.cpp    ${PROJECT_SOURCE_DIR}/src/dapploader.h
    ${PROJECT_SOURCE_DIR}/src/private/dplatformthemeproxy_p.h    ${PROJECT_SOURCE_DIR}/src/dplatformthemeproxy.cpp    ${PROJECT_SOURCE_DIR}/src/dplatformthemeproxy.h
    ${PROJECT_SOURCE_DIR}/src/dqmlappmainwindowinterface.cpp    ${PROJECT_SOURCE_DIR}/src/dqmlappmainwindowinterface.h
    ${PROJECT_SOURCE_DIR}/src/dqmlapppreloadinterface.cpp    ${PROJECT_SOURCE_DIR}/src/dqmlapppreloadinterface.h
    ${PROJECT_SOURCE_DIR}/src/dquickblitframebuffer.cpp    ${PROJECT_SOURCE_DIR}/src/dquickblitframebuffer.h
    ${PROJECT_SOURCE_DIR}/src/dquickitemviewport.cpp    ${PROJECT_SOURCE_DIR}/src/dquickitemviewport.h
    ${PROJECT_SOURCE_DIR}/src/private/dquicksystempalette_p.h    ${PROJECT_SOURCE_DIR}/src/dquicksystempalette.cpp    ${PROJECT_SOURCE_DIR}/src/dquicksystempalette.h
    ${PROJECT_SOURCE_DIR}/src/private/dquickwindow_p.h    ${PROJECT_SOURCE_DIR}/src/dquickwindow.cpp    ${PROJECT_SOURCE_DIR}/src/dquickwindow.h
    ${PROJECT_SOURCE_DIR}/src/private/dquickglobal_p.h
    ${PROJECT_SOURCE_DIR}/src/dtkdeclarative_config.h
    ${PROJECT_SOURCE_DIR}/src/dtkdeclarative_global.h
)

if(EnableQt6)
    qt_add_translation(QM_FILES ${TS_FILES})
endif()

qt_add_qml_module(${LIB_NAME}
    PLUGIN_TARGET ${LIB_NAME}
    URI "org.deepin.dtk"
    VERSION "1.0"
    QML_FILES
        ${QML_DTK}
        ${QML_DTK_AND_QT_CONTROLS}
        ${QML_DTK_CONTROLS}
    SOURCES
        ${DTK_SOURCES}
    OUTPUT_DIRECTORY
        "${PROJECT_BINARY_DIR}/plugins/org/deepin/dtk"
    RESOURCES
        ${ASSETS_RCS}
)

if(EnableQt6)
    qt_add_shaders(${LIB_NAME} "_shaders_ng"
        BATCHABLE
        PRECOMPILE
        PREFIX
            "/dtk/declarative"
        FILES
            "shaders_ng/quickitemviewport.vert"
            "shaders_ng/quickitemviewport.frag"
            "shaders_ng/quickitemviewport-opaque.vert"
            "shaders_ng/quickitemviewport-opaque.frag"
            "shaders_ng/cornerscolorshader.vert"
            "shaders_ng/cornerscolorshader.frag"
            "shaders_ng/shadowmaterial.vert"
            "shaders_ng/shadowmaterial.frag"
    )
endif()

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
PRIVATE
    Qt${QT_VERSION_MAJOR}::QuickPrivate
    Qt${QT_VERSION_MAJOR}::DBus
    PkgConfig::GL
)

target_include_directories(${LIB_NAME} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>
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

