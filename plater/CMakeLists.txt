cmake_minimum_required(VERSION 2.8)
project(plater)
find_package (Threads)

set (CMAKE_CXX_FLAGS "-std=c++11 -Wall -pedantic -U__STRICT_ANSI__ -pthread")

set(ALL_SOURCES
    stl/Point3.cpp
    stl/Face.cpp
    stl/Volume.cpp
    stl/Model.cpp
    stl/StlFactory.cpp
 
    Bitmap.cpp
    PlacedPart.cpp
    Placer.cpp
    Plate.cpp
    Part.cpp
    QuadTree.cpp
    Rectangle.cpp
    Request.cpp
    Solution.cpp
    Triangle.cpp
    
    sleep.cpp
    log.cpp
    util.cpp
	wingetopt.cpp
    )

include_directories("${CMAKE_SOURCE_DIR}/src")

add_library(libplater ${ALL_SOURCES})

target_link_libraries(libplater ${CMAKE_THREAD_LIBS_INIT})

add_executable(plater "main.cpp")
target_link_libraries(plater libplater ${CMAKE_THREAD_LIBS_INIT})

install(TARGETS plater DESTINATION bin)
