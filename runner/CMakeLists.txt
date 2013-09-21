project(libcppmanip)

cmake_minimum_required(VERSION 2.8)

set(LIBCPPMANIP_INSTALLATION_INCLUDE "" CACHE FILEPATH "Path to libcppmanip installation include")
set(LIBCPPMANIP_INSTALLATION_LIB "" CACHE FILEPATH "Path to libcppmanip installation lib")

add_definitions("-std=c++11")

include_directories(${LIBCPPMANIP_INSTALLATION_INCLUDE} "src" "../gmock-1.6.0/fused-src")
link_directories(${LIBCPPMANIP_INSTALLATION_LIB})
link_directories("${PROJECT_SOURCE_DIR}/../gmock-1.6.0/build/gtest")
link_directories("${PROJECT_SOURCE_DIR}/../gmock-1.6.0/build")

add_library(cppmaniprunnercore STATIC
    src/CommandLineParser.cpp
)

add_executable(cppmaniprunnertest
    test/main.cpp
    test/CommandLineParserTest.cpp
)

target_link_libraries(cppmaniprunnertest cppmaniprunnercore gtest gmock pthread)

add_executable(cppmaniprunner_extract_function src/extract_function.cpp)

target_link_libraries(cppmaniprunner_extract_function cppmanip cppmaniprunnercore boost_serialization)
