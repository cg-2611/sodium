#include "sodium/nac/error/error.h"

nac::Exception::Exception(std::string message) {
    message_ = "[nac]: " + message;
}

const char * nac::Exception::what() const noexcept {
    return message_.c_str();
}
