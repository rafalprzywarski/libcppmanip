#include "CommandLineParser.hpp"
#include <sstream>

ExtractMethodRequest CommandLineParser::parseExtractMethod(int argc, const char** argv) 
{
    ExtractMethodRequest req;
    req.sourceFilename = argv[1];
    for (int i = 2; i < argc; i += 3)
    {
        ExtractMethodRequest::Location loc;
        loc.extractedMethodName = argv[i];
        loc.sourceSelection.from = to_u(argv[i + 1]);
        loc.sourceSelection.to = to_u(argv[i + 2]);
        req.locations.push_back(loc);
    }
    return req;
}

unsigned int CommandLineParser::to_u(const char* s) 
{
    std::istringstream ss(s);
    unsigned u;
    ss >> u;
    return u;
}
