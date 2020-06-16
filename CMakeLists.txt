cmake_minimum_required(VERSION 2.8)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -std=c++11 -lpthread")

set(ROOT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")
set(EXECUTABLE_OUTPUT_PATH ${ROOT_DIRECTORY}/bin)

add_subdirectory(src/dflog)

add_subdirectory(example)
