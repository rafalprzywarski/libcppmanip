include_directories("${PROJECT_SOURCE_DIR}/include" "src" "${PROJECT_SOURCE_DIR}/gmock-1.6.0/fused-src")

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
