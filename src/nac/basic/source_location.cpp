#include "sodium/nac/basic/source_location.h"

#include "sodium/nac/basic/source_range.h"

namespace sodium {

SourceLocation::SourceLocation() : line_(0), column_(0), position_(nullptr) {}

SourceLocation::SourceLocation(int line, int column, const char *position)
        : line_(line),
          column_(column),
          position_(position) {}

int SourceLocation::line() const {
    return line_;
}

int SourceLocation::column() const {
    return column_;
}

const char *SourceLocation::position() const {
    return position_;
}

bool SourceLocation::is_valid() const {
    return line_ > 0 && column_ > 0 && position_ != nullptr;
}

SourceRange SourceLocation::to(SourceLocation location) const {
    return SourceRange(*this, location);
}

std::string SourceLocation::to_string() const {
    return std::string(std::to_string(line_) + ":" + std::to_string(column_));
}

} // namespace sodium
