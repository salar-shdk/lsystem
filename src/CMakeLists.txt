cmake_minimum_required(VERSION 3.22)
project(lsystem-src)

set(HEADER_LIST lsystem.h)

set(SOURCE_LIST lsystem.cpp)

set(TEST_SOURCE_LIST main.cpp)

add_library(lsystem SHARED ${SOURCE_LIST})

install(TARGETS lsystem DESTINATION lib)
install(FILES lsystem.h DESTINATION include)

add_executable(lsystem-test ${TEST_SOURCE_LIST})
target_link_libraries(lsystem-test lsystem)
