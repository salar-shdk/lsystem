cmake_minimum_required(VERSION 3.22)
project(lsystem)

set(COMPILER "GCC" CACHE STRING "GCC")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -pedantic -Wall -Wold-style-cast")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wdisabled-optimization")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wfloat-equal -Wnon-virtual-dtor -Woverloaded-virtual")

add_subdirectory(src)
