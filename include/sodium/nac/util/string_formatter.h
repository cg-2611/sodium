#ifndef SODIUM_NAC_UTIL_STRING_FORMATTER_H
#define SODIUM_NAC_UTIL_STRING_FORMATTER_H

#include <cstdio>
#include <string>
#include <utility>

namespace nac {
namespace StringFormatter {

template<typename... Args>
static std::string formatString(const std::string &format, Args &&...args) {
    const int size = std::snprintf(nullptr, 0, format.c_str(), args...) + 1;

    std::unique_ptr<char[]> buffer = std::make_unique<char[]>(size);
    std::snprintf(buffer.get(), size, format.c_str(), args...);

    return std::string(buffer.get(), size - 1);
}


} // namespace StringFormatter
} // namespace nac

#endif // SODIUM_NAC_UTIL_STRING_FORMATTER_H
