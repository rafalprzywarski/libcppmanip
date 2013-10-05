#include "LocalVariablesAssert.hpp"
#include <gtest/gtest.h>
#include <algorithm>

namespace cppmanip
{
namespace query
{
namespace test
{

void expectEqUnordered(ast::LocalVariables found, ast::LocalVariables expected)
{
    std::sort(found.begin(), found.end());
    std::sort(expected.begin(), expected.end());
    ASSERT_TRUE(found == expected);
}

void expectEqOrdered(ast::LocalVariables found, ast::LocalVariables expected)
{
    ASSERT_TRUE(found == expected);
}

}
}
}
