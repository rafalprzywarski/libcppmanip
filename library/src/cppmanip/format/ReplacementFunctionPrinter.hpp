#ifndef CPPMANIP_8205A435770C426D9594A56914DB1E5F_HPP
#define CPPMANIP_8205A435770C426D9594A56914DB1E5F_HPP
#include <string>
#include <cppmanip/ast/Statement.hpp>

namespace cppmanip
{
namespace format
{

struct ReplacementFunction
{
    std::string definition, call;
};

class ReplacementFunctionPrinter
{
public:
    virtual ~ReplacementFunctionPrinter() { }
    virtual ReplacementFunction printFunctionFromStmts(const std::string& name, ast::StatementRange stmts) = 0;
};

typedef std::shared_ptr<ReplacementFunctionPrinter> ReplacementFunctionPrinterPtr;

}
}
#endif // CPPMANIP_8205A435770C426D9594A56914DB1E5F_HPP
