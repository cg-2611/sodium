#ifndef SODIUM_NAC_BASIC_SOURCE_LOCATION_H
#define SODIUM_NAC_BASIC_SOURCE_LOCATION_H

#include <string>

namespace sodium {

class SourceRange;

/// Used to represent a location in a source file.
class SourceLocation {
public:
    /// Constructor for SourceLocation object.
    /// Creates an invalid SourceLocation with line and column numbers of -1 and a position of \c nullptr;
    SourceLocation();

    /// Construct for SourceLocation.
    /// \param line The line number of the location.
    /// \param column The column number of the location.
    /// \param position Pointer to the character at the location.
    SourceLocation(uint32_t line, uint32_t column, const char *position);

    /// \return The line number of \c this location.
    [[nodiscard]] uint32_t line() const;

    /// \return  The column number of \c this location.
    [[nodiscard]] uint32_t column() const;

    /// \return The position of \c this location.
    [[nodiscard]] const char *position() const;

    /// \return \c true  if \c this location is a valid location in a Sodium source file.
    [[nodiscard]] bool is_valid() const;

    [[nodiscard]] SourceRange to(SourceLocation location) const;

    [[nodiscard]] std::string to_string() const;

private:
    uint32_t line_;
    uint32_t column_;
    const char *position_;
};

} // namespace sodium

#endif // SODIUM_NAC_BASIC_SOURCE_LOCATION_H
