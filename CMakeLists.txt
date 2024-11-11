cmake_minimum_required(VERSION 3.10)

project(ProgRedesP1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(lib)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../bin)

add_executable(server src/server.cpp src/game.cpp)
add_executable(client src/client.cpp src/game.cpp)
