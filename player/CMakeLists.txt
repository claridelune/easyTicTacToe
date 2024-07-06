cmake_minimum_required(VERSION 3.28)

set(SOURCES
    player.cpp
    main.cpp
)

# add_subdirectory(../shared)

add_executable(clientPlayer ${SOURCES})
target_include_directories(clientPlayer PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(clientPlayer PUBLIC nlohmann_json sharedLibrary)
