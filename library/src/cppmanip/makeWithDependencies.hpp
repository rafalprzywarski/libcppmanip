#ifndef CPPMANIP_E0950E118FE34D8FB6CF2866B3A0504D_HPP
#define CPPMANIP_E0950E118FE34D8FB6CF2866B3A0504D_HPP
#include <memory>

namespace cppmanip
{

template <typename Container, typename Object>
std::shared_ptr<Object> makeWithDependencies(Object Container::*object)
{
    auto container = std::make_shared<Container>();
    return std::shared_ptr<Object>(container, &((*container).*object));
}

}
#endif // CPPMANIP_E0950E118FE34D8FB6CF2866B3A0504D_HPP
