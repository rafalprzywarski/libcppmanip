#include "CppManip.hpp"
#include <clang/Tooling/CommonOptionsParser.h>
#include <clang/Tooling/Tooling.h>
#include "TextFileOps.hpp"
#include "ClangToolArgsBuilder.hpp"
#include "MethodExtractorFrontendActionFactory.hpp"
#include "TextOperationApplier.hpp"
#include "TextReplacer.hpp"
#include "TextReplacementRecorder.hpp"
#include "OffsetConverter.hpp"
#include "SourceLocationConverter.hpp"

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

void applySourceOperationsToFile(SourceReplacements replacements, const std::string& filename)
{
    std::string source = loadTextFromFile(filename);
    SourceLocationConverter sourceLocationConverter(source);

    TextReplacer replacer(source);
    for (auto r : replacements)
        replacer.replaceWithTextInRange(
            r.text, sourceLocationConverter.getOffsetFromLocation(r.from), sourceLocationConverter.getOffsetFromLocation(r.to));

    std::string modifiedSource = replacer.getText();
    writeTextToFile(modifiedSource, filename); 
}

SourceReplacements extractFunctionInFile(const std::string& functionName, SourceSelection selection, const std::string& filename)
{
    TextOperationApplier sourceOperations;
    std::string source = loadTextFromFile(filename);
    SourceLocationConverter sourceLocationConverter(source);
    SourceRange range;
    range.from = sourceLocationConverter.getOffsetFromLocation(selection.from);
    range.to = sourceLocationConverter.getOffsetFromLocation(selection.to);
    MethodExtractorFrontendActionFactory factory(functionName, range, sourceOperations);
    performFrontendActionForFile(factory, filename);
    OffsetConverter offsetCoverter(source);
    TextReplacementRecorder recorder(std::bind(&OffsetConverter::getLocationFromOffset, &offsetCoverter, std::placeholders::_1));
    sourceOperations.apply(recorder);
    return recorder.getReplacements();
}

void extractMethodInFile(const std::string& methodName, SourceRange range, const std::string& filename)
{
    std::string source = loadTextFromFile(filename);
    OffsetConverter offsetCoverter(source);
    SourceSelection selection;
    selection.from = offsetCoverter.getLocationFromOffset(range.from);
    selection.to = offsetCoverter.getLocationFromOffset(range.to);
    auto replacements = extractFunctionInFile(methodName, selection, filename);
    applySourceOperationsToFile(replacements, filename);
}