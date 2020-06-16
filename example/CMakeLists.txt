cmake_minimum_required(VERSION 2.8)

aux_source_directory(. SOURCE_FILES)

include_directories(${ROOT_DIRECTORY}/src)

add_executable(example ${SOURCE_FILES})

target_link_libraries(example dflog)
