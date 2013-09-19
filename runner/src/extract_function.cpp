#include "CommandLineParser.hpp"
#include <CppManip.hpp>
#include <ExtractMethodError.hpp>
#include "../library/src/OffsetConverter.hpp"
#include <../library/src/TextFileOps.hpp>
#include <stdexcept>
#include <iostream>
#include <boost/range/algorithm_ext/push_back.hpp>
#include <boost/archive/xml_oarchive.hpp>
#include <fstream>
#include <SourceReplacementSerialization.hpp>

const std::string REPLACEMENTS_FILE = "replacements.xml";

void saveReplacements(SourceReplacements replacements)
{
    std::ofstream of(REPLACEMENTS_FILE);
    boost::archive::xml_oarchive oa(of);
    oa << boost::serialization::make_nvp("replacements", replacements);
}

int main(int argc, const char** argv)
{
    try
    {
        CommandLineParser parser;
        auto req = parser.parseExtractMethod(argc, argv);
        SourceReplacements replacements;
        OffsetConverter conv(loadTextFromFile(req.sourceFilename));
        try
        {
            for (auto loc : req.locations)
            {
                SourceSelection selection;
                selection.from = conv.getLocationFromOffset(loc.sourceSelection.from);
                selection.to = conv.getLocationFromOffset(loc.sourceSelection.to);
                boost::push_back(replacements, extractFunctionInFile(loc.extractedMethodName, selection, req.sourceFilename));
            }
        }
        catch (const ExtractMethodError& e)
        {
            std::cerr << e.what() << std::endl;
            return 1;
        }
        saveReplacements(replacements);
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
