set(BIN_NAME qml-inspect)

find_package(Qt5 COMPONENTS Quick QuickControls2)
# find_package(DtkCore REQUIRED)
find_package(DtkGui REQUIRED)

add_executable(${BIN_NAME} 
    ./main.cpp 
    ./qml.qrc
    ./dciicons.qrc
)

target_include_directories(${BIN_NAME} PUBLIC
    ${Qt5Quick_INCLUDE_DIRS}
    ${Qt5QuickControls2_INCLUDE_DIRS}
    # ${DtkCore_INCLUDE_DIR}
    ${DtkGui_INCLUDE_DIR}
)

target_link_libraries(${BIN_NAME} PUBLIC
    Qt5::Quick 
    Qt5::QuickControls2
    # ${DtkCore_LIBRARIES}
    ${DtkGui_LIBRARIES}
    dtkdeclarative
)

#install(FILES ${BIN_NAME} DESTINATION "${CMAKE_INSTALL_PREFIX}/bin")
