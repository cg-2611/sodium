#include "sodium/nac/errors/error.h"

#include <string_view>

#include "sodium/nac/basic/source_location.h"
#include "sodium/nac/util/string_formatter.h"

namespace sodium {

// returns the corresponding string describing the kind of error passed
static std::string_view get_error_message(ErrorKind kind);

Error::Error(ErrorKind kind, SourceLocation location) : message_("error") {
    std::string error_message(get_error_message(kind));
    message_ +=
        StringFormatter::format_string(" @ %lu:%lu: %s", location.line(), location.column(), error_message.c_str());
}

const std::string &Error::message() const {
    return message_;
}

static std::string_view get_error_message(ErrorKind kind) {
    switch (kind) {
        case ErrorKind::SYNTAX_ERROR: return "syntax error";
        case ErrorKind::UNRECOGNISED_TOKEN: return "unrecognised token";
        default: return "unkown error";
    }
}

} // namespace sodium
