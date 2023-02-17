cmake_minimum_required(VERSION 3.22)
project(lsystem-src)

set(HEADER_LIST Lsystem.h)

set(SOURCE_LIST Lsystem.cpp)

set(TEST_SOURCE_LIST main.cpp)

add_library(lsystem SHARED ${SOURCE_LIST})

add_executable(lsystem-test ${TEST_SOURCE_LIST})
target_link_libraries(lsystem-test lsystem)
