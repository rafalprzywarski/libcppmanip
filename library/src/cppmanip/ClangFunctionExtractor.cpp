#include "ClangFunctionExtractor.hpp"
#include "ClangToolArgsBuilder.hpp"
#include "SourceRange.hpp"
#include "SourceLocationConverter.hpp"
#include "TextOperationApplier.hpp"
#include "OffsetConverter.hpp"
#include "TextReplacementRecorder.hpp"
#include "io/TextFileOps.hpp"
#include "MethodExtractorFrontendActionFactory.hpp"
#include <clang/Tooling/Tooling.h>
#include <clang/Tooling/CommonOptionsParser.h>

namespace cppmanip
{

void performFrontendActionForFile(clang::tooling::FrontendActionFactory& actionFactory, std::string sourceFilename)
{
    ClangToolArgsBuilder args;
    args.setSourceFilename(sourceFilename);
    auto argc = args.getArgc();
    auto argv = args.getArgv();
    clang::tooling::CommonOptionsParser parser(argc, argv);
    clang::tooling::ClangTool tool(parser.GetCompilations(), { sourceFilename }); // clang bug: parser.GetSourcePathList() uses statics and "adds up" source file names
    tool.run(&actionFactory);
}

SourceRange getSourceRange(SourceSelection selection, const std::string& source)
{
    SourceLocationConverter sourceLocationConverter(source);
    SourceRange range;
    range.from = sourceLocationConverter.getOffsetFromLocation(selection.from);
    range.to = sourceLocationConverter.getOffsetFromLocation(selection.to);
    return range;
}

SourceReplacements recordReplacements(const OffsetBasedTextOperationApplier& sourceOperations, const std::string& source)
{
    OffsetConverter offsetCoverter(source);
    TextReplacementRecorder recorder(std::bind(&OffsetConverter::getLocationFromOffset, &offsetCoverter, std::placeholders::_1));
    sourceOperations.apply(recorder);
    return recorder.getReplacements();
}

SourceReplacements ClangFunctionExtractor::extractFunctionInFile(const std::string& functionName, SourceSelection selection, const std::string& filename)
{
    std::string source = io::loadTextFromFile(filename);
    OffsetBasedTextOperationApplier sourceOperations;
    MethodExtractorFrontendActionFactory factory(functionName, getSourceRange(selection, source), sourceOperations);
    performFrontendActionForFile(factory, filename);
    return recordReplacements(sourceOperations, source);
}

}

