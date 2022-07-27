#include "sodium/nac/exceptions/exception.h"

namespace nac {

// returns a message describing the argument error
static constexpr std::string_view getErrorMessage(Error error);

Exception::Exception(Error error) : message_("[nac]: error: ") {
    message_ += getErrorMessage(error);
}

static constexpr std::string_view getErrorMessage(Error error) {
    switch (error) {
        case Error::FILE_OPEN_FAIL: return "failed to open file";
        case Error::FILE_READ_FAIL: return "failed to read contents of file";
        case Error::UNRECOGNISED_TOKEN: return "unrecognised token";
        default: return "unknown error code";
    }
}

} // namespace nac
