cmake_minimum_required(VERSION 3.16)
project(donix)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)

add_executable(donix src/main.cpp src/mcpServer.cpp)


