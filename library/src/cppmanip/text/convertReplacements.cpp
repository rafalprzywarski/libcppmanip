#include "convertReplacements.hpp"
#include <boost/range/algorithm/transform.hpp>

namespace cppmanip
{
namespace text
{

boundary::SourceReplacements convertReplacements(text::Replacements<ast::SourceOffset> replacements, ConvertOffsetToSourceLocation conv)
{
    boundary::SourceReplacements converted;
    boost::transform(replacements, std::back_inserter(converted), [=](text::Replacement<ast::SourceOffset> r) {
        boundary::SourceReplacement cr;
        cr.from = conv(r.from);
        cr.to = conv(r.to);
        cr.text = r.text;
        return cr;
    });
    return converted;
}

}
}
