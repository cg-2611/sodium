#include "sodium/nac/exceptions/exception.h"

namespace nac {

Exception::Exception(const Error error) : message_("[nac]: error: ") {
    message_ += getErrorMessage(error);
}

constexpr std::string_view Exception::getErrorMessage(Error error) {
    switch (error) {
        case Error::FILE_OPEN_FAIL: return "failed to open file";
        case Error::FILE_READ_FAIL: return "failed to read contents of file";
        case Error::UNRECOGNISED_TOKEN: return "unrecognised token";
        default: return "unknown error code";
    }
}

} // namespace nac
