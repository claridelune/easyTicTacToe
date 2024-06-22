cmake_minimum_required(VERSION 3.28)

set(SOURCES
    main.cpp
    logger.cpp
    server.cpp
)

add_executable(serverMain ${SOURCES})

target_include_directories(serverMain PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})