project(libcppmanip)

cmake_minimum_required(VERSION 2.8)

add_library(gtestgmock gmock-1.6.0/fused-src/gmock-gtest-all.cc)

include_directories("src" "gmock-1.6.0/fused-src")

add_executable(cppmaniptest test/main.cpp) 

target_link_libraries(cppmaniptest gtestgmock pthread)

add_executable(cppmanip src/main.cpp)
