cmake_minimum_required(VERSION 3.28)

project(easyTicTacToe)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
# set(CMAKE_CXX_COMPILER clang++)

set(SOURCES
    mainServer.cpp
)
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

add_subdirectory(lib/json)
add_subdirectory(shared)
add_subdirectory(server_main)
add_subdirectory(server_trainer)
add_subdirectory(player)
add_subdirectory(admin)
add_subdirectory(neural_network)
add_subdirectory(udp)

add_executable(MainServer ${SOURCES})

target_include_directories(MainServer PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/server_main)
target_link_libraries(MainServer PRIVATE TcpServerLibrary UdpLibrary sharedLibrary Threads::Threads)
