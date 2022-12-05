cmake_minimum_required(VERSION 3.23)
project(memAllocator)

set(CMAKE_CXX_STANDARD 14)

add_executable(memAllocator main.cpp error_strings.h logger.cpp fast_malloc.cpp fast_malloc.h )
