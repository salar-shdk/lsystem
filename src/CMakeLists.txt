cmake_minimum_required(VERSION 3.22)
project(lsystem-src)

# Add gperftools
find_package(Gperftools REQUIRED)
if (GPERFTOOLS_FOUND)
  message("Found gperftools: ${GPERFTOOLS_INCLUDE_DIRS}")
  include_directories(${GPERFTOOLS_INCLUDE_DIRS})
endif()

# Add profiling flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-omit-frame-pointer -O2")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -lprofiler")

set(HEADER_LIST lsystem.h)

set(SOURCE_LIST lsystem.cpp)

set(TEST_SOURCE_LIST main.cpp)

add_library(lsystem SHARED ${SOURCE_LIST})

install(TARGETS lsystem DESTINATION lib)
install(FILES lsystem.h DESTINATION include)

add_executable(lsystem-test ${TEST_SOURCE_LIST})
target_link_libraries(lsystem-test lsystem ${GPERFTOOLS_PROFILER})
target_link_directories(lsystem-test PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/src/)
