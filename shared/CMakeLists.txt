cmake_minimum_required(VERSION 3.28)

add_library(sharedLibrary
    database.cpp
    utils.cpp
)

target_include_directories(sharedLibrary PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(sharedLibrary PUBLIC nlohmann_json)