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
    src/cppmanip/io/TextFileOps.cpp
    src/cppmanip/clangutil/ClangToolArgsBuilder.cpp
    src/cppmanip/CppManip.cpp
    src/cppmanip/PrettyFunctionPrinter.cpp
    src/cppmanip/NaiveStatementLocator.cpp
    src/cppmanip/DelayedMethodExtractor.cpp
    src/cppmanip/NaiveLocalVariableLocator.cpp
    src/cppmanip/TextReplacementRecorder.cpp
    src/cppmanip/OffsetConverter.cpp
    src/cppmanip/SourceLocationConverter.cpp
    src/cppmanip/ClangFunctionExtractor.cpp
    src/cppmanip/ClangFunctionExtractorFactory.cpp
    src/cppmanip/DefaultClangFunctionLocator.cpp
    src/cppmanip/TranslationUnitFunctionExtractor.cpp
    src/cppmanip/TranslationUnitFunctionExtractorFactory.cpp
    src/cppmanip/clangutil/runTranslationUnitHandlerOnFile.cpp
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
    test/cppmanip/ParsedFunction.cpp
    test/cppmanip/Condition.cpp
    test/main.cpp
    test/cppmanip/TextOperationApplierTest.cpp
    test/cppmanip/OffsetRangeTest.cpp
    test/cppmanip/clangutil/ClangToolArgsBuilderTest.cpp
    test/cppmanip/SourceExtractorTest.cpp
    test/cppmanip/TextReplacementRecorderTest.cpp
    test/cppmanip/OffsetConverterTest.cpp
    test/cppmanip/SourceLocationConverterTest.cpp
    test/cppmanip/DefaultClangFunctionLocatorTest.cpp
) 

target_link_libraries(cppmaniptest cppmanip gtest gmock boost_thread boost_system pthread)
