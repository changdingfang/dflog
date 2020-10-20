cmake_minimum_required(VERSION 2.8)

aux_source_directory(. SOURCE_FILES)

include_directories(${DFLOG_SRC})

add_library(fmt STATIC ${SOURCE_FILES})

