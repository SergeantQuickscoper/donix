cmake_minimum_required(VERSION 3.16)
project(donix)

set(CMAKE_CXX_STANDARD 17)

# Add llama.cpp
add_subdirectory(external/llama.cpp)

# llama.cpp link
target_link_libraries(donix PRIVATE llama)

