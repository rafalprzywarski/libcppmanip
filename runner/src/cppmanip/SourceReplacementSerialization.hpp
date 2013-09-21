#ifndef SOURCEREPLACEMENTSERIALIZATION_HPP
#define SOURCEREPLACEMENTSERIALIZATION_HPP
#include <SourceReplacement.hpp>
#include <boost/serialization/vector.hpp>

namespace boost {
namespace serialization {

template<class Archive>
void serialize(Archive & ar, SourceLocation& loc, unsigned)
{
    ar & boost::serialization::make_nvp("col", loc.col);
    ar & boost::serialization::make_nvp("row", loc.row);
}

template<class Archive>
void serialize(Archive & ar, SourceReplacement& r, unsigned)
{
    ar & boost::serialization::make_nvp("from", r.from);
    ar & boost::serialization::make_nvp("to", r.to);
    ar & boost::serialization::make_nvp("text", r.text);
}

}
}

#endif // SOURCEREPLACEMENTSERIALIZATION_HPP
