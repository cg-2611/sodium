#ifndef SODIUM_NAC_BASIC_SOURCE_RANGE_H
#define SODIUM_NAC_BASIC_SOURCE_RANGE_H

#include "sodium/nac/basic/source_location.h"

namespace sodium {

/// Used to represent a range between a start SourceLocation and end SourceLocation in Sodium source file.
class SourceRange {
public:
    /// Constructor for SourceRange.
    /// Both start and end are default initialized.
    SourceRange() = default;

    /// Constructor for SourceRange.
    /// \param start The starting SourceLocation of the range.
    /// \param end The ending SourceLocation of the range.
    SourceRange(SourceLocation start, SourceLocation end);

    /// \return The start of \c this range.
    [[nodiscard]] SourceLocation start() const;

    /// \return The end of \c this range.
    [[nodiscard]] SourceLocation end() const;

private:
    SourceLocation start_;
    SourceLocation end_;
};

} // namespace sodium

#endif // SODIUM_NAC_BASIC_SOURCE_RANGE_H
