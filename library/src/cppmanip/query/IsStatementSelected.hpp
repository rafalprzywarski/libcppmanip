#ifndef CPPMANIP_5A947F6A312B465FAAABC0ED20F62CED_HPP
#define CPPMANIP_5A947F6A312B465FAAABC0ED20F62CED_HPP
#include <cppmanip/ast/Statement.hpp>
#include <functional>

namespace cppmanip {

namespace query {

typedef std::function<bool(ast::StatementPtr)> IsStatementSelected;

}
}

#endif // CPPMANIP_5A947F6A312B465FAAABC0ED20F62CED_HPP
