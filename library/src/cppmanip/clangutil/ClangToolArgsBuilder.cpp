#include "ClangToolArgsBuilder.hpp"
#include <algorithm>

namespace cppmanip
{
namespace clangutil
{

ClangToolArgsBuilder::ClangToolArgsBuilder() 
        : args{"", "", "--"}, rawArgs(args.size()) { }

void ClangToolArgsBuilder::setSourceFilename(const std::string& filename) 
{
    args[1] = filename;
}

int ClangToolArgsBuilder::getArgc() const
{
    return args.size();
}

const char** ClangToolArgsBuilder::getArgv() const 
{
    std::transform(args.begin(), args.end(), rawArgs.begin(), std::mem_fn(&std::string::c_str));
    return &rawArgs[0]; 
}

}
}
