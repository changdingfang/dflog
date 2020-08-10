cmake_minimum_required(VERSION 2.8)
project(dflog)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -std=c++11 -lpthread")

set(ROOT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")
set(EXECUTABLE_OUTPUT_PATH ${ROOT_DIRECTORY}/build/bin)
set(LIBRARY_OUTPUT_PATH ${ROOT_DIRECTORY}/build/lib)

option(DFLOG_TEST "build test dflog" OFF)

message("... chekc")
message("dflog test: ${DFLOG_TEST}")

add_subdirectory(src/fmt)
add_subdirectory(src/dflog)
add_subdirectory(example)

if (DFLOG_TEST)
    message("... test")
    add_subdirectory(tests)
endif()
