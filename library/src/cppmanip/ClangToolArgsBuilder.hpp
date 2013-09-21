#ifndef CPPMANIP_8A600DD0C25B4B6EBEDD227EC5617E12_HPP
#define CPPMANIP_8A600DD0C25B4B6EBEDD227EC5617E12_HPP
#include <vector>
#include <string>

namespace cppmanip
{

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

}
#endif // CPPMANIP_8A600DD0C25B4B6EBEDD227EC5617E12_HPP
