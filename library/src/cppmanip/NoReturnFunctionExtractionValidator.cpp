#include "NoReturnFunctionExtractionValidator.hpp"
#include <cppmanip/boundary/ExtractMethodError.hpp>
#include <boost/range/adaptor/transformed.hpp>
#include <boost/algorithm/string/join.hpp>
#include <boost/range/algorithm_ext/push_back.hpp>
#include <boost/range/algorithm/sort.hpp>

namespace cppmanip
{

namespace
{

std::string printOrderedVariableNameList(const ast::LocalVariables& variables)
{
    using boost::adaptors::transformed;
    std::vector<std::string> names;
    boost::push_back(names, variables | transformed(std::bind(&ast::LocalVariable::getName, std::placeholders::_1)));
    boost::sort(names);
    return boost::algorithm::join(names, ", ");
}

}

void NoReturnFunctionExtractionValidator::validateStatements(const std::string& functionName, ast::ScopedStatementRange selected)
{
    auto used = findVariablesDeclaredByAndUsedAfterStmts(selected.getRange(), selected.getScope());
    if (!used.empty())
        throw boundary::ExtractMethodError("Cannot extract \'" + functionName +
            "\'. Following variables are in use after the selected statements: " + printOrderedVariableNameList(used));
}

}
