#include "LocalVariablesAssert.hpp"
#include <gtest/gtest.h>
#include <algorithm>

namespace cppmanip
{
namespace query
{
namespace test
{

void expectEqUnordered(ast::LocalVariables found, std::vector<ast::LocalVariable> expected)
{
    auto order = [](ast::LocalVariable left, ast::LocalVariable right) { return left.getNameWithType() < right.getNameWithType(); };
    auto orderPtr = [order](ast::LocalVariablePtr left, ast::LocalVariablePtr right) { return order(*left, *right); };
    std::sort(found.begin(), found.end(), orderPtr);
    std::sort(expected.begin(), expected.end(), order);
    expectEqOrdered(found, expected);
}

void expectEqOrdered(ast::LocalVariables found, std::vector<ast::LocalVariable> expected)
{
    ASSERT_EQ(expected.size(), found.size());
    for (decltype(found.size()) i = 0; i < found.size(); ++i)
    {
        ASSERT_EQ(expected[i].getName(), found[i]->getName());
        ASSERT_EQ(expected[i].getNameWithType(), found[i]->getNameWithType());
    }
}
}
}
}
