cmake_minimum_required(VERSION 3.28)

set(SOURCES
    admin.cpp
    player.cpp
    trainer.cpp
    config.cpp
    manager.cpp
)

add_library(TcpServerLibrary ${SOURCES})

target_include_directories(TcpServerLibrary PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(TcpServerLibrary PUBLIC nlohmann_json sharedLibrary)