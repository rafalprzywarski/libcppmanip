#include "formatStatements.hpp"
#include <sstream>

namespace cppmanip
{
namespace format
{

std::string formatStatements(ast::StatementRange stmts, unsigned indentationWidth)
{
    std::ostringstream ss;
    for (auto s : stmts)
        ss << std::string(indentationWidth, ' ') << s->getSourceCode() << "\n";
    return ss.str();
}

}
}
