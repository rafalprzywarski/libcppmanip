project(libcppmanip)

cmake_minimum_required(VERSION 2.8)

add_definitions("-std=c++11")

set(LLVM_CONFIG "llvm-config-3.2")

execute_process(COMMAND ${LLVM_CONFIG} --cxxflags OUTPUT_VARIABLE LLVM_CXX_FLAGS OUTPUT_STRIP_TRAILING_WHITESPACE)
execute_process(COMMAND ${LLVM_CONFIG} --ldflags OUTPUT_VARIABLE LLVM_LINKER_FLAGS OUTPUT_STRIP_TRAILING_WHITESPACE)
add_definitions("${LLVM_CXX_FLAGS} -fexceptions")
set(CMAKE_CXX_LINK_FLAGS "${CMAKE_CXX_LINK_FLAGS} ${LLVM_LINKER_FLAGS}")
set(CMAKE_SHARED_LIBRARY_CREATE_CXX_FLAGS "${CMAKE_SHARED_LIBRARY_CREATE_CXX_FLAGS} ${LLVM_LINKER_FLAGS}")

include_directories("include" "src" "../gmock-1.6.0/fused-src")
link_directories("${PROJECT_SOURCE_DIR}/../gmock-1.6.0/build/gtest")
link_directories("${PROJECT_SOURCE_DIR}/../gmock-1.6.0/build")

add_library(cppmanip SHARED
    src/cppmanip/SourceExtractor.cpp
    src/cppmanip/TextFileOps.cpp
    src/cppmanip/TextOperationApplier.cpp
    src/cppmanip/ClangToolArgsBuilder.cpp
    src/cppmanip/CppManip.cpp
    src/cppmanip/MethodExtractorASTConsumer.cpp
    src/cppmanip/MethodExtractorFrontendAction.cpp
    src/cppmanip/MethodExtractorFrontendActionFactory.cpp
    src/cppmanip/PrettyFunctionPrinter.cpp
    src/cppmanip/NaiveStatementLocator.cpp
    src/cppmanip/MethodExtractorVisitor.cpp
    src/cppmanip/DelayedMethodExtractor.cpp
    src/cppmanip/NaiveLocalVariableLocator.cpp
    src/cppmanip/TextReplacer.cpp
    src/cppmanip/TextReplacementRecorder.cpp
    src/cppmanip/OffsetConverter.cpp
    src/cppmanip/SourceLocationConverter.cpp
)

target_link_libraries(cppmanip LINK_PRIVATE
    clangTooling.a clangFrontend.a clangFrontendTool.a
    clangSerialization.a clangDriver.a
    clangParse.a clangSema.a clangAnalysis.a
    clangRewriteFrontend.a clangRewriteCore.a clangEdit.a clangAST.a
    clangLex.a clangBasic.a clang.a LLVM-3.2)

install(TARGETS cppmanip LIBRARY DESTINATION lib)
install(FILES
        "${PROJECT_SOURCE_DIR}/include/CppManip.hpp"
        DESTINATION include)
install(FILES
        "${PROJECT_SOURCE_DIR}/include/cppmanip/SourceSelection.hpp"
        "${PROJECT_SOURCE_DIR}/include/cppmanip/ExtractMethodError.hpp"
        "${PROJECT_SOURCE_DIR}/include/cppmanip/SourceReplacement.hpp"
        "${PROJECT_SOURCE_DIR}/include/cppmanip/SourceLocation.hpp"
        DESTINATION include/cppmanip)

add_executable(cppmaniptest
    test/ParsedFunction.cpp
    test/Condition.cpp
    test/main.cpp
    test/TextOperationApplierTest.cpp
    test/OffsetRangeTest.cpp
    test/ClangToolArgsBuilderTest.cpp
    test/SourceExtractorTest.cpp
    test/TextReplacementRecorderTest.cpp
    test/OffsetConverterTest.cpp
    test/SourceLocationConverterTest.cpp
) 

target_link_libraries(cppmaniptest cppmanip gtest gmock boost_thread boost_system pthread)
