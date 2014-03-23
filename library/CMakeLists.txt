project(libcppmanip)

cmake_minimum_required(VERSION 2.8)

add_definitions("-std=c++11")

set(LLVM_CONFIG "llvm-config-3.2")

execute_process(COMMAND ${LLVM_CONFIG} --cxxflags OUTPUT_VARIABLE LLVM_CXX_FLAGS OUTPUT_STRIP_TRAILING_WHITESPACE)
execute_process(COMMAND ${LLVM_CONFIG} --ldflags OUTPUT_VARIABLE LLVM_LINKER_FLAGS OUTPUT_STRIP_TRAILING_WHITESPACE)
add_definitions("${LLVM_CXX_FLAGS} -fexceptions -Wall -Werror=return-type -Wno-unused-local-typedefs -fno-strict-aliasing")
set(CMAKE_CXX_LINK_FLAGS "${CMAKE_CXX_LINK_FLAGS} ${LLVM_LINKER_FLAGS}")
set(CMAKE_SHARED_LIBRARY_CREATE_CXX_FLAGS "${CMAKE_SHARED_LIBRARY_CREATE_CXX_FLAGS} ${LLVM_LINKER_FLAGS}")

include_directories("include" "src" "../gmock-1.7.0/fused-src")
link_directories("${PROJECT_SOURCE_DIR}/../gmock-1.7.0/build/gtest")
link_directories("${PROJECT_SOURCE_DIR}/../gmock-1.7.0/build")

add_library(cppmanip SHARED
    src/cppmanip/io/TextFileOps.cpp
    src/cppmanip/clangutil/ClangToolArgsBuilder.cpp
    src/cppmanip/extractFunctionInFile.cpp
    src/cppmanip/text/OffsetConverter.cpp
    src/cppmanip/text/SourceLocationConverter.cpp
    src/cppmanip/clangutil/getFunctionFromAstInSelection.cpp
    src/cppmanip/clangutil/runTranslationUnitHandlerOnFile.cpp
    src/cppmanip/query/findSelectedStatementsInFunction.cpp
    src/cppmanip/clangutil/getStmtOffsetRange.cpp
    src/cppmanip/clangutil/getStmtSpecificOffsetRanges.cpp
    src/cppmanip/query/findLocalVariablesRequiredForStmts.cpp
    src/cppmanip/query/findVariablesDeclaredByAndUsedAfterStmts.cpp
    src/cppmanip/query/isStatementSelected.cpp
    src/cppmanip/format/printFunction.cpp
    src/cppmanip/clangutil/getFunctionStatements.cpp
    src/cppmanip/clangutil/AstGateway.cpp
    src/cppmanip/DefaultFunctionExtractor.cpp
    src/cppmanip/DefaultFunctionExtractorFactory.cpp
    src/cppmanip/FileBasedStatementLocator.cpp
    src/cppmanip/NoReturnFunctionExtractionValidator.cpp
    src/cppmanip/text/convertReplacements.cpp
    src/cppmanip/format/DefaultReplacementFunctionPrinter.cpp
    src/cppmanip/DefaultFunctionReplacer.cpp
    src/cppmanip/format/formatStatements.cpp
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
        "${PROJECT_SOURCE_DIR}/include/cppmanip/boundary/SourceSelection.hpp"
        "${PROJECT_SOURCE_DIR}/include/cppmanip/boundary/ExtractMethodError.hpp"
        "${PROJECT_SOURCE_DIR}/include/cppmanip/boundary/SourceReplacement.hpp"
        "${PROJECT_SOURCE_DIR}/include/cppmanip/boundary/SourceLocation.hpp"
        "${PROJECT_SOURCE_DIR}/include/cppmanip/boundary/extractFunctionInFile.hpp"
        DESTINATION include/cppmanip/boundary)

add_executable(cppmaniptest
    test/cppmanip/clangutil/ParsedFunction.cpp
    test/cppmanip/clangutil/Condition.cpp
    test/main.cpp
    test/cppmanip/text/StrictOperationRecorderTest.cpp
    test/cppmanip/math/PositionRangeTest.cpp
    test/cppmanip/clangutil/ClangToolArgsBuilderTest.cpp
    test/cppmanip/text/OffsetConverterTest.cpp
    test/cppmanip/clangutil/getFunctionFromAstInSelectionTest.cpp
    test/cppmanip/text/SourceLocationConverterTest.cpp
    test/cppmanip/query/findSelectedStatementsInFunctionTest.cpp
    test/cppmanip/clangutil/getStmtOffsetRangeTest.cpp
    test/cppmanip/query/findLocalVariablesRequiredForStmtsTest.cpp
    test/cppmanip/clangutil/getASTContextForSource.cpp
    test/cppmanip/query/findVariablesDeclaredByAndUsedAfterStmtsTest.cpp
    test/cppmanip/format/printFunctionTest.cpp
    test/cppmanip/query/LocalVariablesAssert.cpp
    test/cppmanip/clangutil/getFunctionStatementsTest.cpp
    test/cppmanip/query/AstFactories.cpp
    test/cppmanip/text/convertReplacementsTest.cpp
    test/cppmanip/format/formatStatementsTest.cpp
    test/cppmanip/format/DefaultReplacementFunctionPrinterTest.cpp
    test/cppmanip/query/isStatementSelectedTest.cpp
) 

target_link_libraries(cppmaniptest cppmanip gtest gmock boost_thread boost_system pthread)
