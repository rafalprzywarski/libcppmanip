#ifndef CLANGTOOLARGSBUILDER_HPP
#define CLANGTOOLARGSBUILDER_HPP
#include <vector>
#include <string>

class ClangToolArgsBuilder
{
public:
    ClangToolArgsBuilder();
    void setSourceFilename(const std::string& filename);
    int getArgc() const;
    const char **getArgv() const;
private:
    std::vector<std::string> args;
    mutable std::vector<const char *> rawArgs;
};

#endif // CLANGTOOLARGSBUILDER_HPP
