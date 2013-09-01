project(libcppmanip)

cmake_minimum_required(VERSION 2.8)

set(LLVM_CONFIG "llvm-config-3.2")

execute_process(COMMAND ${LLVM_CONFIG} --cxxflags OUTPUT_VARIABLE LLVM_CXX_FLAGS OUTPUT_STRIP_TRAILING_WHITESPACE)
execute_process(COMMAND ${LLVM_CONFIG} --ldflags OUTPUT_VARIABLE LLVM_LINKER_FLAGS OUTPUT_STRIP_TRAILING_WHITESPACE)
add_definitions("${LLVM_CXX_FLAGS} -fexceptions -std=c++11")
set(CMAKE_CXX_LINK_FLAGS "${CMAKE_CXX_LINK_FLAGS} ${LLVM_LINKER_FLAGS}")

add_library(gtestgmock gmock-1.6.0/fused-src/gmock-gtest-all.cc)

include_directories("src" "gmock-1.6.0/fused-src")

add_library(cppmanip STATIC src/SourceExtractor.cpp src/TextFileOps.cpp src/CommandLineParser.cpp src/TextOperationApplier.cpp)

add_executable(cppmaniptest test/main.cpp test/TextOperationApplierTest.cpp test/OffsetRangeTest.cpp) 

target_link_libraries(cppmaniptest cppmanip gtestgmock pthread)

add_executable(cppmaniprunner src/main.cpp)

target_link_libraries(cppmaniprunner cppmanip clangTooling.a clangFrontend.a clangFrontendTool.a
    clangSerialization.a clangDriver.a
    clangParse.a clangSema.a clangAnalysis.a
    clangRewriteFrontend.a clangRewriteCore.a clangEdit.a clangAST.a
    clangLex.a clangBasic.a clang.a LLVM-3.2)
