cmake_minimum_required(VERSION 2.8)
project(plater)

set(ALL_SOURCES
    modelFile/modelFile.cpp
    modelFile/logoutput.cpp
    )

include_directories("${CMAKE_SOURCE_DIR}/src")

add_executable(plater "main.cpp" ${ALL_SOURCES})
install(TARGETS plater DESTINATION bin)
