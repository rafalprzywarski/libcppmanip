include_directories("../include" "src" "../gmock-1.6.0/fused-src")

add_library(cppmaniprunnercore STATIC src/CommandLineParser.cpp)

add_executable(cppmaniprunnertest
    test/main.cpp
    test/CommandLineParserTest.cpp)

target_link_libraries(cppmaniprunnertest cppmaniprunnercore gtest gmock pthread)

add_executable(cppmaniprunner src/main.cpp)

target_link_libraries(cppmaniprunner cppmanip cppmaniprunnercore clangTooling.a clangFrontend.a clangFrontendTool.a
    clangSerialization.a clangDriver.a
    clangParse.a clangSema.a clangAnalysis.a
    clangRewriteFrontend.a clangRewriteCore.a clangEdit.a clangAST.a
    clangLex.a clangBasic.a clang.a LLVM-3.2)
