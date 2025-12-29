include(${PROJECT_SOURCE_DIR}/src/targets.cmake)

find_package(Qt${QT_VERSION_MAJOR}QuickCompiler)

# Quick compiler gen qmlc
file(GLOB QML_RCS ${PROJECT_SOURCE_DIR}/src/dtkdeclarative_qml.qrc)
qtquick_compiler_add_resources(RESOURCES
    ${QML_RCS}
)
# Add translation source file
qt5_add_translation(QM_FILES ${TS_FILES})

add_library(${LIB_NAME} SHARED
    ${RESOURCES}
    ${QM_FILES}
    ${PROJECT_SOURCE_DIR}/src/dtkdeclarative_shaders.qrc
)

dtk_extend_target(${LIB_NAME} EnableCov ${ENABLE_COV})
target_compile_definitions(${LIB_NAME} PRIVATE
    D_IGNORE_DEPRECATIONS
)

set_target_properties(${LIB_NAME} PROPERTIES
    VERSION ${DTK_VERSION}
    SOVERSION ${DTK_VERSION_MAJOR}
    EXPORT_NAME Declarative
)

target_link_libraries(${LIB_NAME}
PUBLIC
    ${LIB_NAME}_properties
PRIVATE
    ${LIB_NAME}_sources
)

# Install translations
install(FILES ${QM_FILES} DESTINATION "${TRANSLATIONS_INSTALL_PATH}")

# Install library
install(TARGETS ${LIB_NAME} EXPORT Dtk${DTK_NAME_SUFFIX}DeclarativeTargets DESTINATION "${LIB_INSTALL_DIR}")
# Install export targets
install(EXPORT Dtk${DTK_NAME_SUFFIX}DeclarativeTargets NAMESPACE Dtk${DTK_NAME_SUFFIX}:: FILE Dtk${DTK_NAME_SUFFIX}DeclarativeTargets.cmake DESTINATION "${CONFIG_INSTALL_DIR}")
