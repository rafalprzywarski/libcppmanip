#include "CommandLineParser.hpp"
#include <CppManip.hpp>
#include <stdexcept>
#include <iostream>

int main(int argc, const char** argv)
{
    try
    {
        CommandLineParser parser;
        auto req = parser.parseExtractMethod(argc, argv);
        extractMethodInFile(req.extractedMethodName, req.sourceSelection, req.sourceFilename);
    }
    catch (const std::logic_error& e)
    {
        std::cerr << "Internal error: " << e.what() << std::endl;
        return 1;
    }
    catch (...)
    {
        std::cerr << "Unknown internal error" << std::endl;
        return 1;
    }
}