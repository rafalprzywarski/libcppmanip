#include <cppmanip/runner/CommandLineParser.hpp>
#include <CppManip.hpp>
#include <stdexcept>
#include <iostream>
#include <boost/range/algorithm_ext/push_back.hpp>
#include <boost/archive/xml_oarchive.hpp>
#include <fstream>
#include <cppmanip/SourceReplacementSerialization.hpp>

using namespace cppmanip::runner;

const std::string REPLACEMENTS_FILE = "replacements.xml";
const std::string ERROR_FILE = "errors.xml";

void saveReplacements(cppmanip::SourceReplacements replacements)
{
    std::ofstream of(REPLACEMENTS_FILE);
    boost::archive::xml_oarchive oa(of);
    oa << boost::serialization::make_nvp("replacements", replacements);
}

void saveError(const std::string& error)
{
    std::ofstream of(ERROR_FILE);
    boost::archive::xml_oarchive oa(of);
    oa << boost::serialization::make_nvp("error", error);
}

ExtractFunctionRequest parseRequest(int argc, const char** argv)
{
    CommandLineParser parser;
    return parser.parseExtractFunction(argc, argv);
}

void extractFunctions(ExtractFunctionRequest req)
{
    cppmanip::SourceReplacements allReplacements;
    try
    {
        for (auto loc : req.locations)
        {
            auto replacements = cppmanip::extractFunctionInFile(loc.extractedMethodName, loc.sourceSelection, req.sourceFilename);
            boost::push_back(allReplacements, replacements);
        }
        saveReplacements(allReplacements);
    }
    catch (const cppmanip::boundary::ExtractMethodError& e)
    {
        saveError(e.what());
    }
}

int main(int argc, const char** argv)
{
    try
    {
        auto req = parseRequest(argc, argv);
        extractFunctions(req);
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
