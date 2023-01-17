cmake_minimum_required(VERSION 3.23)
project(memAllocator)

set(CMAKE_CXX_STANDARD 14)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -D FIRST_FIT ")

add_executable(memAllocator main.cpp error_strings.h logger.cpp MemMalloc.cpp MemMalloc.h list.h map.h)
