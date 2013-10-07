#ifndef CPPMANIP_DC40597E542F4CDB995BD64C51CDB200_HPP
#define CPPMANIP_DC40597E542F4CDB995BD64C51CDB200_HPP
#include <memory>

namespace cppmanip
{
namespace text
{

template <typename Position>
struct Replacement
{
    Position from, to;
    std::string text;
    Replacement() : from(), to() { }
    Replacement(Position from, Position to, const std::string& text)
        : from(from), to(to), text(text) { }
};

template <typename Position>
using Replacements = std::vector<Replacement<Position>>;

template <typename Position>
class OperationRecorder
{
public:
    virtual ~OperationRecorder() { }
    virtual void insertTextAt(const std::string& text, Position pos) = 0;
    virtual void removeTextInRange(Position from, Position to) = 0;
    virtual Replacements<Position> getReplacements() const = 0;
};

typedef OperationRecorder<unsigned> OffsetBasedOperationRecorder;
typedef std::shared_ptr<OffsetBasedOperationRecorder> OffsetBasedOperationRecorderPtr;

}
}
#endif // CPPMANIP_DC40597E542F4CDB995BD64C51CDB200_HPP
