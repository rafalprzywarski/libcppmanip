set(LLVM_CONFIG "llvm-config-3.2")

execute_process(COMMAND ${LLVM_CONFIG} --cxxflags OUTPUT_VARIABLE LLVM_CXX_FLAGS OUTPUT_STRIP_TRAILING_WHITESPACE)
execute_process(COMMAND ${LLVM_CONFIG} --ldflags OUTPUT_VARIABLE LLVM_LINKER_FLAGS OUTPUT_STRIP_TRAILING_WHITESPACE)
add_definitions("${LLVM_CXX_FLAGS} -fexceptions")
set(CMAKE_CXX_LINK_FLAGS "${CMAKE_CXX_LINK_FLAGS} ${LLVM_LINKER_FLAGS}")

include_directories("../include" "src" "../gmock-1.6.0/fused-src")

add_library(cppmanip STATIC
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
)

add_executable(cppmaniptest
    test/main.cpp
    test/TextOperationApplierTest.cpp
    test/OffsetRangeTest.cpp
    test/ClangToolArgsBuilderTest.cpp
) 

target_link_libraries(cppmaniptest cppmanip gtest gmock pthread)
