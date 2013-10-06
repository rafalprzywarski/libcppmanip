#ifndef CPPMANIP_5FEE80539C8E4819AD394E1CC73C6592_HPP
#define CPPMANIP_5FEE80539C8E4819AD394E1CC73C6592_HPP
#include <cppmanip/ast/Gateway.hpp>

namespace cppmanip
{
namespace clangutil
{

class AstGateway : public ast::Gateway
{
public:
    virtual ast::FunctionPtr getFunctionInSelectionFromFile(ast::SourceOffsetRange selection, const std::string& filename);
};

}
}
#endif // ASTGATEWAY_HPP
