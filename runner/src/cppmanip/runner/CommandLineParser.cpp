#include "CommandLineParser.hpp"
#include <sstream>
#include <boost/lexical_cast.hpp>

namespace cppmanip
{
namespace runner
{

ExtractFunctionRequest CommandLineParser::parseExtractFunction(int argc, const char** argv) 
{
    ExtractFunctionRequest req;
    req.sourceFilename = argv[1];
    for (int i = 2; i < argc; i += 5)
    {
        ExtractFunctionRequest::Location loc;
        loc.extractedMethodName = argv[i];
        loc.sourceSelection.from.row = to_u(argv[i + 1]);
        loc.sourceSelection.from.col = to_u(argv[i + 2]);
        loc.sourceSelection.to.row = to_u(argv[i + 3]);
        loc.sourceSelection.to.col = to_u(argv[i + 4]);
        req.locations.push_back(loc);
    }
    return req;
}

unsigned int CommandLineParser::to_u(const char* s) 
{
    return boost::lexical_cast<unsigned>(s);
}

}
}
