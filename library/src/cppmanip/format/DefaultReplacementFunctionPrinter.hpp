#ifndef DEFAULTREPLACEMENTFUNCTIONPRINTER_HPP
#define DEFAULTREPLACEMENTFUNCTIONPRINTER_HPP
#include "ReplacementFunctionPrinter.hpp"

namespace cppmanip
{
namespace format
{

class DefaultReplacementFunctionPrinter : public ReplacementFunctionPrinter
{
public:
    virtual ReplacementFunction printFunctionFromStmts(const std::string& name, ast::StatementRange stmts);
};

}
}
#endif // DEFAULTREPLACEMENTFUNCTIONPRINTER_HPP
