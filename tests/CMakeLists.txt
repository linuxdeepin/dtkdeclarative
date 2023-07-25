set(BIN_NAME unit-test)

find_package(Dtk${DTK_VERSION_MAJOR}Core REQUIRED)
find_package(Dtk${DTK_VERSION_MAJOR}Gui REQUIRED)
find_package(GTest REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Gui Quick QuickControls2 Qml Test)

file(GLOB TEST_SOURCES
    ut_colorselector.cpp
    ut_dconfigwrapper.cpp
    ut_dqmlglobalobject.cpp
    ut_dapploader.cpp
    ut_thememenu.cpp
    ut_windowbuttongroup.cpp
    ut_dquickrectangle.cpp
    ut_dquickwindow.cpp
    ut_waterprogressbar.cpp
    ut_keysequencelistener.cpp
)
if (EnableDtk5)
    list(APPEND TEST_SOURCES
        ut_dplatformthemeproxy.cpp
    )
endif()

add_executable(${BIN_NAME}
    main.cpp ${TEST_SOURCES}
    ${CMAKE_CURRENT_LIST_DIR}/data.qrc
)

target_compile_options(${BIN_NAME} PRIVATE
    "-fno-access-control"
)

target_compile_definitions(${BIN_NAME} PRIVATE
    QML_PLUGIN_PATH="${PLUGIN_OUTPUT_DIR}"
    CHAMELEON_PATH="${PLUGIN_OUTPUT_DIR}"
    BIN_NAME="${BIN_NAME}"
)

target_link_libraries(${BIN_NAME} PRIVATE
    Qt${QT_VERSION_MAJOR}::Test
    Qt${QT_VERSION_MAJOR}::QuickControls2
    ${LIB_NAME}
    GTest::GTest
    pthread
    dl
    m
)
