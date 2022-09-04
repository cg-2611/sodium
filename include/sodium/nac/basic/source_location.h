#ifndef SODIUM_NAC_BASIC_SOURCE_LOCATION_H
#define SODIUM_NAC_BASIC_SOURCE_LOCATION_H

namespace sodium {

/**
 * @brief Used to represent a location in a source file.
 *
 */
class SourceLocation {
public:
    /**
     * @brief Construct a new Source Location object.
     *
     * @param line The line number of the location.
     * @param column The column number of the location.
     * @param position Pointer to the character at the location.
     */
    SourceLocation(int line, int column, const char *position);

    /**
     * @brief Get the line number of this location.
     *
     * @return \c int that is the line number of this location.
     */
    int line() const noexcept;

    /**
     * @brief Get the column number of this location.
     *
     * @return \c int that is the column number of this location.
     */
    int column() const noexcept;

    /**
     * @brief Get a pointer to the character at this location.
     *
     * @return \c const \c char* to the character at this location.
     */
    const char *position() const noexcept;

private:
    int line_;
    int column_;
    const char *position_;
};

} // namespace sodium

#endif // SODIUM_NAC_BASIC_SOURCE_LOCATION_H
