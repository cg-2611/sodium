#include "sodium/nac/diagnostics/error.h"

#include <string>

#include "sodium/nac/diagnostics/diagnostic.h"

namespace sodium {

Error::Error(ErrorKind kind, SourceLocation location)
        : Diagnostic(DiagnosticKind::ERROR),
          kind_(kind),
          location_(location) {
    message_ += "error @ " + location_.to_string() + ":";
}

ErrorKind Error::error_kind() const {
    return kind_;
}

SourceLocation Error::location() const {
    return location_;
}

const std::string &Error::message() const {
    return message_;
}

} // namespace sodium
