#ifndef CPPMANIP_C20DF98AB97E467387AA8DD21F900A0E_HPP
#define CPPMANIP_C20DF98AB97E467387AA8DD21F900A0E_HPP
#include <CppManip.hpp>
#include <string>
#include <vector>

namespace cppmanip
{
namespace runner
{

struct ExtractFunctionRequest
{
    struct Location
    {
        std::string extractedMethodName;
        SourceSelection sourceSelection;
    };
    std::string sourceFilename;
    std::vector<Location> locations;
};

class CommandLineParser
{
public:
    ExtractFunctionRequest parseExtractFunction(int argc, const char** argv);
private:
    static unsigned to_u(const char *s);
};

}
}

#endif // CPPMANIP_C20DF98AB97E467387AA8DD21F900A0E_HPP
