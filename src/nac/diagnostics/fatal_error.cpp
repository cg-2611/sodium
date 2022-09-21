#include "sodium/nac/diagnostics/fatal_error.h"

#include <cerrno>
#include <string_view>

namespace sodium {

FatalError::FatalError(FatalErrorKind kind, const std::string &path) : Diagnostic(DiagnosticKind::FATAL), kind_(kind) {
    message_ += "error: ";
    message_ += description_from_kind(kind_);
    message_ += " \'" + path + "\'";

    // if errno is set, append the errno string to message_
    if (errno != 0) {
        message_ += ": ";
        message_ += std::strerror(errno);
    }
}

FatalErrorKind FatalError::kind() const {
    return kind_;
}

const std::string &FatalError::message() const {
    return message_;
}

constexpr std::string_view FatalError::description_from_kind(FatalErrorKind kind) {
    switch (kind) {
        case FatalErrorKind::FILE_READ_FAIL: return "failed to read file";
        default: return "unkown fatal error";
    }
}

} // namespace sodium
