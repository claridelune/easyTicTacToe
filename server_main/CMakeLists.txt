cmake_minimum_required(VERSION 3.28)

set(SOURCES
    main.cpp
    admin.cpp
    player.cpp
    trainer.cpp
    config.cpp
    logger.cpp
    manager.cpp
    socket.cpp
)

add_executable(serverMain ${SOURCES})

target_include_directories(serverMain PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} SharedLibrary)
target_link_libraries(serverMain PRIVATE nlohmann_json)