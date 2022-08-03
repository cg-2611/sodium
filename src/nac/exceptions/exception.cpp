#include "sodium/nac/exceptions/exception.h"

#include <string_view>

#include "sodium/nac/errors/error_manager.h"

namespace nac {

// returns the corresponding string describing the kind of exception passed
static std::string_view getExceptionMessage(ExceptionKind kind);

Exception::Exception(ExceptionKind kind) : message_("[nac]: ") {
    message_ += getExceptionMessage(kind);
}

const char *Exception::what() const noexcept {
    return message_.c_str();
}

static std::string_view getExceptionMessage(ExceptionKind kind) {
    switch (kind) {
        case ExceptionKind::ERRORS_GENERATED: return ErrorManager::errorMessages();
        case ExceptionKind::FILE_OPEN_FAIL: return "error: failed to open file";
        case ExceptionKind::FILE_READ_FAIL: return "error: failed to read contents of file";
        default: return "unkown exception";
    }
}

} // namespace nac
