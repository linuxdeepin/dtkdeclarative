
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CHAMELEON_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../chameleon/imports")
#set(CHAMELEON_PATH "/tmp/chameleon-qml/imports")
add_definitions(-DCHAMELEON_PATH=\"${CHAMELEON_PATH}\")
#message("CHAMELEON_PATH : ${CHAMELEON_PATH}")
# 打开 Qt DEPRECATED 警告
add_definitions(-DQT_DEPRECATED_WARNINGS)

add_subdirectory(exhibition)
add_subdirectory(qml-inspect)
