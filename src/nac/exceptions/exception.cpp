#include "sodium/nac/exceptions/exception.h"

namespace nac {

Exception::Exception(std::string message) {
    message_ = "[nac]: " + message;
}

const char* Exception::what() const noexcept {
    return message_.c_str();
}

} // namespace nac
