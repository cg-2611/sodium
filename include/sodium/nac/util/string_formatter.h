#ifndef SODIUM_NAC_UTIL_STRING_FORMATTER_H
#define SODIUM_NAC_UTIL_STRING_FORMATTER_H

#include <cstdio>
#include <memory>
#include <string>

namespace sodium::StringFormatter {

/// @brief Performs string formatting similar to std::sprintf().
/// @tparam Args Variadic template arguments that are used to populate the formatted string.
/// @param format The format for the returned string.
/// @param args The arguments to populate the formatted string with.
/// @return std::string that is the formatted string using format and args.
template<typename... Args>
static std::string format_string(const std::string &format, Args &&...args) {
    // get the size of the formatted string, +1 for '\0'
    const int SIZE = snprintf(nullptr, 0, format.c_str(), args...) + 1;

    std::unique_ptr<char[]> buffer = std::make_unique<char[]>(SIZE);
    snprintf(buffer.get(), SIZE, format.c_str(), args...);

    return std::string(buffer.get(), SIZE - 1); // -1 to remove '\0'
}

} // namespace sodium::StringFormatter

#endif // SODIUM_NAC_UTIL_STRING_FORMATTER_H
