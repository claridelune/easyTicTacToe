cmake_minimum_required(VERSION 3.28)

set(SOURCES
    dataHandler.cpp
    neuralNetwork.cpp
)

add_library(neuralNetworkLibrary ${SOURCES})

target_include_directories(neuralNetworkLibrary PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})