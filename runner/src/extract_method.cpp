#include "CommandLineParser.hpp"
#include <CppManip.hpp>
#include <ExtractMethodError.hpp>
#include <stdexcept>
#include <iostream>

int main(int argc, const char** argv)
{
    try
    {
        CommandLineParser parser;
        auto req = parser.parseExtractMethod(argc, argv);
        for (auto loc : req.locations)
        {
            try
            {
                extractMethodInFile(loc.extractedMethodName, loc.sourceSelection, req.sourceFilename);
            }
            catch (const ExtractMethodError& e)
            {
                std::cerr << e.what() << std::endl;
                return 1;
            }
        }
    }
    catch (const std::exception& e)
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