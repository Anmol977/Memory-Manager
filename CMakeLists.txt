cmake_minimum_required(VERSION 3.23)
project(memAllocator)

set(CMAKE_CXX_STANDARD 14)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -D NEXT_FIT ")

add_executable(memAllocator main.cpp error_strings.h logger.cpp fast_malloc.cpp fast_malloc.h list.h map.h)
