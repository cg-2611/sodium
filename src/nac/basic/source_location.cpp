#include "sodium/nac/basic/source_location.h"

namespace sodium {

SourceLocation::SourceLocation(int line, int column, const char *position)
        : line_(line),
          column_(column),
          position_(position) {}

int SourceLocation::line() const noexcept {
    return line_;
}

int SourceLocation::column() const noexcept {
    return column_;
}

const char *SourceLocation::position() const noexcept {
    return position_;
}

} // namespace sodium
