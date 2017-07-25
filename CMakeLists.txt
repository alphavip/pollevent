cmake_minimum_required(VERSION 2.8)

project(test)

set(CMAKE_CXX_FLAGS "-DLINUX -DDEBUG -g -ggdb -std=c++11 -Wall")

include_directories(loop/)

add_subdirectory(loop)

add_executable(test test.cpp)

target_link_libraries(test libloop)
