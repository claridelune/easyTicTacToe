cmake_minimum_required(VERSION 3.28)

set(SOURCES
    admin.cpp
    main.cpp
)

# add_subdirectory(../shared)

add_executable(clientAdmin ${SOURCES})
target_include_directories(clientAdmin PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(clientAdmin PUBLIC nlohmann_json sharedLibrary)
