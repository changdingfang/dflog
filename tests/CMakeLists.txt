cmake_minimum_required(VERSION 2.8)

enable_testing()

include_directories(${ROOT_DIRECTORY}/src)
include_directories(${ROOT_DIRECTORY}/tests/googletest/include)

file(GLOB_RECURSE testfiles RELATIVE "${ROOT_DIRECTORY}/tests" ${ROOT_DIRECTORY}/tests/*.cpp)

foreach(testfile IN LISTS testfiles)
	get_filename_component(testname ${testfile} NAME_WE)
	add_executable(${testname} ${testfile})
	target_link_libraries(${testname} dflog gtest fmt)
	message(STATUS "test file name : ${testname}")
endforeach()
