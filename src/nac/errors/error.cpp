#include "sodium/nac/errors/error.h"

#include <cstdio>
#include <string_view>

namespace nac {

static std::string_view errorMessage(ErrorKind kind);

Error::Error(ErrorKind kind, size_t line, size_t column) : message_("error") {
    const size_t mLength = std::to_string(line).size() + std::to_string(column).size() + 6;
    std::string m(mLength, '\0');
    std::sprintf(m.data(), " @ %lu:%lu: ", line, column);
    message_ += m;

    message_ += errorMessage(kind);
}

const std::string &Error::getMessage() const {
    return message_;
}

static std::string_view errorMessage(ErrorKind kind) {
    switch (kind) {
        case ErrorKind::UNRECOGNISED_TOKEN: return "unrecognised token";
        default: return "unkown error";
    }
}

} // namespace nac
