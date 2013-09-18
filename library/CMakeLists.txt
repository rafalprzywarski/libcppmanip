set(LLVM_CONFIG "llvm-config-3.2")

execute_process(COMMAND ${LLVM_CONFIG} --cxxflags OUTPUT_VARIABLE LLVM_CXX_FLAGS OUTPUT_STRIP_TRAILING_WHITESPACE)
execute_process(COMMAND ${LLVM_CONFIG} --ldflags OUTPUT_VARIABLE LLVM_LINKER_FLAGS OUTPUT_STRIP_TRAILING_WHITESPACE)
add_definitions("${LLVM_CXX_FLAGS} -fexceptions")
set(CMAKE_CXX_LINK_FLAGS "${CMAKE_CXX_LINK_FLAGS} ${LLVM_LINKER_FLAGS}")
set(CMAKE_SHARED_LIBRARY_CREATE_CXX_FLAGS "${CMAKE_SHARED_LIBRARY_CREATE_CXX_FLAGS} ${LLVM_LINKER_FLAGS}")

include_directories("../include" "src" "../gmock-1.6.0/fused-src")

add_library(cppmanip SHARED
    src/SourceExtractor.cpp
    src/TextFileOps.cpp
    src/TextOperationApplier.cpp
    src/ClangToolArgsBuilder.cpp
    src/CppManip.cpp
    src/MethodExtractorASTConsumer.cpp
    src/MethodExtractorFrontendAction.cpp
    src/MethodExtractorFrontendActionFactory.cpp
    src/PrettyFunctionPrinter.cpp
    src/NaiveStatementLocator.cpp
    src/MethodExtractorVisitor.cpp
    src/DelayedMethodExtractor.cpp
    src/NaiveLocalVariableLocator.cpp
    src/TextReplacer.cpp
    src/TextReplacementRecorder.cpp
    src/OffsetConverter.cpp
    src/SourceLocationConverter.cpp
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
        "${PROJECT_SOURCE_DIR}/include/SourceSelection.hpp"
        "${PROJECT_SOURCE_DIR}/include/ExtractMethodError.hpp"
        "${PROJECT_SOURCE_DIR}/include/SourceReplacement.hpp"
        "${PROJECT_SOURCE_DIR}/include/SourceLocation.hpp"
        "${PROJECT_SOURCE_DIR}/include/SourceRange.hpp"
        DESTINATION include)

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
