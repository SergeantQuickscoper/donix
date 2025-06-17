cmake_minimum_required(VERSION 3.16)
project(donix)

set(CMAKE_CXX_STANDARD 17)

find_package(Boost 1.70 REQUIRED COMPONENTS system process)

include_directories(include)

add_executable(donix src/main.cpp src/mcpServer.cpp src/mcpClient.cpp)

target_include_directories(donix PRIVATE ${Boost_INCLUDE_DIRS})

target_link_libraries(donix PRIVATE Boost::system Boost::process)

