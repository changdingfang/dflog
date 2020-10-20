cmake_minimum_required(VERSION 2.8)

aux_source_directory(. SOURCE_FILES)

include_directories(${DFLOG_SRC})

add_library(dflog STATIC ${SOURCE_FILES})

target_link_libraries(dflog fmt)
