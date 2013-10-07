#include "DefaultFunctionExtractor.hpp"
#include "query/findLocalVariablesRequiredForStmts.hpp"
#include "query/findSelectedStatementsInFunction.hpp"
#include "query/findVariablesDeclaredByAndUsedAfterStmts.hpp"
#include "format/printFunction.hpp"
#include "text/StrictOperationRecorder.hpp"
#include "text/TextReplacementRecorder.hpp"
#include "text/OffsetConverter.hpp"
#include "text/SourceLocationConverter.hpp"
#include "io/TextFileOps.hpp"
#include "clangutil/AstGateway.hpp"
#include "FileBasedStatementLocator.hpp"
#include <cppmanip/boundary/ExtractMethodError.hpp>
#include <boost/algorithm/string/join.hpp>
#include <boost/range/algorithm_ext/push_back.hpp>
#include <boost/range/algorithm/sort.hpp>
#include <boost/range/adaptor/transformed.hpp>

namespace cppmanip
{

std::vector<std::string> getVariableNames(const ast::LocalVariables& variables)
{
    using boost::adaptors::transformed;
    std::vector<std::string> names;
    boost::push_back(names, variables | transformed(std::bind(&ast::LocalVariable::getName, std::placeholders::_1)));
    return names;
}

class ReplacementRecorder
{
public:
    ReplacementRecorder(const std::string& filename) : filename(filename) { }
    virtual void insertTextAt(const std::string& text, ast::SourceOffset pos)
    {
        applier.insertTextAt(text, pos);
    }
    virtual void replaceTextInRange(const std::string& text, ast::SourceOffsetRange range)
    {
        applier.removeTextInRange(range.getFrom(), range.getTo());
        applier.insertTextAt(text, range.getFrom());
    }
    boundary::SourceReplacements getReplacements() const
    {
        text::OffsetConverter offsetCoverter(io::loadTextFromFile(filename));
        text::TextReplacementRecorder recorder([&](unsigned offset) { return offsetCoverter.getLocationFromOffset(offset); });
        applier.apply(recorder);
        return recorder.getReplacements();
    }
private:
    text::OffsetBasedStrictOperationRecorder applier;
    std::string filename;
};

std::vector<std::string> getArgumentDeclarations(const ast::LocalVariables& variables)
{
    std::vector<std::string> args;
    for (auto d : variables)
        args.push_back(d->getNameWithType());
    return args;
}


std::string getSource(ast::StatementRange stmts)
{
    std::string source;
    for (auto stmt : stmts)
    {
        source += stmt->getSourceCode();
        if (stmt != stmts.back())
            source += stmt->getSourceCodeAfter();
    }
    return source;
}

struct ReplacementFunction
{
    std::string definition, call;
};

ReplacementFunction printReplacementFunctionFromStmts(const std::string& functionName, ast::StatementRange selected)
{
    auto required = query::findLocalVariablesRequiredForStmts(selected);
    return {
        format::printFunctionDefinition("void", functionName, getArgumentDeclarations(required), getSource(selected)),
        format::printFunctionCall(functionName, getVariableNames(required)) };
}

void defineFunction(const std::string& definition, ast::FunctionPtr originalFunction, ReplacementRecorder& recorder)
{
    recorder.insertTextAt(definition, originalFunction->getDefinitionOffset());
}

ast::SourceOffsetRange getRange(ast::StatementRange stmts)
{
    return { stmts.front()->getRange().getFrom(), stmts.back()->getRange().getTo() };
}

void replaceStmtsWithCall(ast::StatementRange stmts, const std::string& call, ReplacementRecorder& recorder)
{
    recorder.replaceTextInRange(call, getRange(stmts));
}

boundary::SourceReplacements DefaultFunctionExtractor::extractFunctionFromSelectionInFile(
    const std::string& functionName, boundary::SourceSelection selection, const std::string& filename)
{
    auto selected = stmtLocator->getSelectedFunctionAndStmts(selection);
    validator->validateStatements(functionName, selected.stmts, selected.function);
    auto replacementFunction = printReplacementFunctionFromStmts(functionName, selected.stmts);
    ReplacementRecorder recorder(filename);
    defineFunction(replacementFunction.definition, selected.function, recorder);
    replaceStmtsWithCall(selected.stmts, replacementFunction.call, recorder);
    return recorder.getReplacements();
}

}
