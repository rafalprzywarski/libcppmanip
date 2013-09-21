#ifndef CPPMANIP_650003D942654C61B6F65F46A45BDDD7_HPP
#define CPPMANIP_650003D942654C61B6F65F46A45BDDD7_HPP
#include <cppmanip/SourceReplacement.hpp>
#include <boost/serialization/vector.hpp>

namespace boost {
namespace serialization {

template<class Archive>
void serialize(Archive & ar, cppmanip::SourceLocation& loc, unsigned)
{
    ar & boost::serialization::make_nvp("col", loc.col);
    ar & boost::serialization::make_nvp("row", loc.row);
}

template<class Archive>
void serialize(Archive & ar, cppmanip::SourceReplacement& r, unsigned)
{
    ar & boost::serialization::make_nvp("from", r.from);
    ar & boost::serialization::make_nvp("to", r.to);
    ar & boost::serialization::make_nvp("text", r.text);
}

}
}

#endif // CPPMANIP_650003D942654C61B6F65F46A45BDDD7_HPP
