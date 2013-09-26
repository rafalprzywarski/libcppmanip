#include "runTranslationUnitHandlerOnFile.hpp"
#include "ClangToolArgsBuilder.hpp"
#include "FrontendActionWithTranslationUnitHandlerFactory.hpp"
#include <clang/Tooling/CommonOptionsParser.h>
#include <clang/Tooling/Tooling.h>

namespace cppmanip
{
namespace clangutil
{

void runTranslationUnitHandlerOnFile(HandleTranslationUnit handleTranslationUnit, std::string filename)
{
    ClangToolArgsBuilder args;
    args.setSourceFilename(filename);
    auto argc = args.getArgc();
    auto argv = args.getArgv();
    clang::tooling::CommonOptionsParser parser(argc, argv);
    clang::tooling::ClangTool tool(parser.GetCompilations(), { filename }); // clang bug: parser.GetSourcePathList() uses statics and "adds up" source file names
    FrontendActionWithTranslationUnitHandlerFactory actionFactory(handleTranslationUnit);
    tool.run(&actionFactory);
}

}
}
