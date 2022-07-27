#include "sodium/nac/exceptions/io_exception.h"

#include <cerrno>

namespace nac {

IOException::IOException(const Error error, const std::string& filePath) : Exception(error) {
    message_ += " \'" + filePath + "\'";

    // if errno is set, get the errno string and append to message_
    if (errno) {
        message_ += ": ";
        message_ += std::strerror(errno);
    }
}

const char* IOException::what() const noexcept {
    return message_.c_str();
}

} // namespace nac
