#include "sodium/nac/basic/source_range.h"

#include "sodium/nac/basic/source_location.h"

namespace sodium {

SourceRange::SourceRange(SourceLocation start, SourceLocation end) : start_(start), end_(end) {}

SourceLocation SourceRange::start() const {
    return start_;
}

SourceLocation SourceRange::end() const {
    return end_;
}

} // namespace sodium
