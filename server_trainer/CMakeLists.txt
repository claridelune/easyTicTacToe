cmake_minimum_required(VERSION 3.28)

set(SOURCES
    main.cpp
    client.cpp
    server.cpp
    manager.cpp
)

add_executable(serverTrainer ${SOURCES})
target_include_directories(serverTrainer PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(serverTrainer PUBLIC nlohmann_json sharedLibrary)
target_link_libraries(serverTrainer PRIVATE neuralNetworkLibrary UdpLibrary Threads::Threads)
