#ifndef SODIUM_NAC_UTIL_STRING_FORMATTER_H
#define SODIUM_NAC_UTIL_STRING_FORMATTER_H

#include <cstdio>
#include <string>
#include <utility>

namespace sodium {
namespace StringFormatter {

/**
 * Performs string formatting similar to std::sprintf();
 * @param format The format for the returned string.
 * @param args The arguments to populate the formatted string with.
 * @return An std::string of the formatted string using format and args.
 */
template<typename... Args>
static std::string formatString(const std::string &format, Args &&...args) {
    // get the size of the formatted string, +1 for '\0'
    const int size = std::snprintf(nullptr, 0, format.c_str(), args...) + 1;

    std::unique_ptr<char[]> buffer = std::make_unique<char[]>(size);
    std::snprintf(buffer.get(), size, format.c_str(), args...);

    return std::string(buffer.get(), size - 1); // -1 to remove '\0'
}

} // namespace StringFormatter
} // namespace sodium

#endif // SODIUM_NAC_UTIL_STRING_FORMATTER_H
