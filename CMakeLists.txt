cmake_minimum_required(VERSION 2.8)
project(plater)

set (CMAKE_CXX_FLAGS "-std=c++0x -Wall -pedantic")

set(ALL_SOURCES
    modelFile/modelFile.cpp
    modelFile/logoutput.cpp
    Rectangle.cpp
    Request.cpp
    )

include_directories("${CMAKE_SOURCE_DIR}/src")

add_executable(plater "main.cpp" ${ALL_SOURCES})
install(TARGETS plater DESTINATION bin)
