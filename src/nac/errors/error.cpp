#include "sodium/nac/errors/error.h"

#include <cstdio>
#include <string_view>

#include "sodium/nac/util/string_formatter.h"

namespace nac {

static std::string_view getErrorMessage(ErrorKind kind);

Error::Error(ErrorKind kind, size_t line, size_t column) : message_("error") {
    std::string errorMessage(getErrorMessage(kind));
    message_ += StringFormatter::formatString(" @ %lu:%lu: %s", line, column, errorMessage.c_str());
}

const std::string &Error::getMessage() const {
    return message_;
}

static std::string_view getErrorMessage(ErrorKind kind) {
    switch (kind) {
        case ErrorKind::UNRECOGNISED_TOKEN: return "unrecognised token";
        default: return "unkown error";
    }
}

} // namespace nac
