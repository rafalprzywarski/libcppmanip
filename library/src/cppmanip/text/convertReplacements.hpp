#ifndef CPPMANIP_983DCB4227E24249ABF85F5E61344E77_HPP
#define CPPMANIP_983DCB4227E24249ABF85F5E61344E77_HPP
#include <cppmanip/boundary/SourceReplacement.hpp>
#include <cppmanip/ast/SourceOffset.hpp>
#include "OperationRecorder.hpp"
#include <functional>

namespace cppmanip
{
namespace text
{

typedef std::function<boundary::SourceLocation(ast::SourceOffset)> ConvertOffsetToSourceLocation;
boundary::SourceReplacements convertReplacements(text::Replacements<ast::SourceOffset> replacements, ConvertOffsetToSourceLocation conv);

}
}
#endif // CPPMANIP_983DCB4227E24249ABF85F5E61344E77_HPP
