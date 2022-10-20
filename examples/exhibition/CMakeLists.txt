set(BIN_NAME dtk-exhibition)

find_package(Qt5 COMPONENTS Quick QuickControls2)
find_package(DtkCore REQUIRED)
find_package(DtkGui REQUIRED)

qt5_add_resources(QRCS ./assets.qrc ./qml.qrc)

add_executable(${BIN_NAME} 
    main.cpp 
    ${QRCS}
)

target_include_directories(${BIN_NAME} PUBLIC
    ${Qt5Quick_INCLUDE_DIRS}
    ${Qt5QuickControls2_INCLUDE_DIRS}
    ${DtkCore_INCLUDE_DIR}
    ${DtkGui_INCLUDE_DIR}
)

target_link_libraries(${BIN_NAME} PUBLIC
    Qt5::Quick 
    Qt5::QuickControls2
    ${DtkCore_LIBRARIES}
    ${DtkGui_LIBRARIES}
    dtkdeclarative
)

install(TARGETS ${BIN_NAME} DESTINATION "${CMAKE_INSTALL_BINDIR}")
install(FILES ${CMAKE_CURRENT_LIST_DIR}/dtk-exhibition.desktop DESTINATION "${CMAKE_INSTALL_DATADIR}/applications")
