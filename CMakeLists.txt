project(libcppmanip)

cmake_minimum_required(VERSION 2.8)

add_definitions("-std=c++11")

add_subdirectory(library)
add_subdirectory(runner)
add_subdirectory(gmock-1.6.0)