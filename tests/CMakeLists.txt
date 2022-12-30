set(BIN_NAME unit-test)

find_package(Dtk REQUIRED COMPONENTS Core Gui)
find_package(GTest REQUIRED)
find_package(Qt5 REQUIRED COMPONENTS Core Gui Quick Qml Test)

file(GLOB TEST_HEADER  ut_.*h)
file(GLOB TEST_SOURCE  *.cpp)

add_executable(${BIN_NAME}
    ${TEST_HEADER}
    ${TEST_SOURCE}
    ${CMAKE_CURRENT_LIST_DIR}/data.qrc
)

target_compile_definitions(${BIN_NAME} PRIVATE
    PREFIX="${CMAKE_PREFIX_PATH}"
    BIN_NAME="${BIN_NAME}"
)

target_compile_options(${BIN_NAME} PRIVATE
    "-fno-access-control"
)

target_link_libraries(${BIN_NAME} PRIVATE
    Qt5::Gui
    Qt5::Core
    Qt5::CorePrivate
    Qt5::Test
    Qt5::Qml
    Qt5::Quick
    Dtk::Core
    Dtk::Gui
    GTest::GTest
    ${LIB_NAME}
    pthread
    dl
)
