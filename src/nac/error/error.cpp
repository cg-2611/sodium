#include "sodium/nac/error/error.h"

#include <cerrno>

sodium::nac::NACException::NACException(std::string message) {
    std::string errnoString = std::strerror(errno);
    message_ = "[nac]: " + message + ": " + errnoString;
}

const char * sodium::nac::NACException::what() const noexcept {
    return message_.c_str();
}
