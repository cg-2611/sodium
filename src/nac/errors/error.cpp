#include "sodium/nac/errors/error.h"

#include <string_view>

#include "sodium/nac/util/string_formatter.h"

namespace sodium {

// returns the corresponding string describing the kind of error passed
static std::string_view getErrorMessage(ErrorKind kind);

Error::Error(ErrorKind kind, size_t line, size_t column) : message_("error") {
    std::string errorMessage(getErrorMessage(kind));
    message_ += StringFormatter::formatString(" @ %lu:%lu: %s", line, column, errorMessage.c_str());
}

const std::string &Error::message() const {
    return message_;
}

static std::string_view getErrorMessage(ErrorKind kind) {
    switch (kind) {
        case ErrorKind::SYNTAX_ERROR: return "syntax error";
        case ErrorKind::UNRECOGNISED_TOKEN: return "unrecognised token";
        default: return "unkown error";
    }
}

} // namespace sodium
