cmake_minimum_required(VERSION 3.28)

set(SOURCES
    udp.cpp
)

add_library(UdpLibrary ${SOURCES})

target_include_directories(UdpLibrary PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(UdpLibrary PRIVATE sharedLibrary)