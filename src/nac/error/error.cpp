#include "sodium/nac/error/error.h"

sodium::nac::NACException::NACException(int code, std::string message) {
    code_ = code;
    message_ = message;
}

const char * sodium::nac::NACException::what() const noexcept {
    return message_.c_str();
}
