#include "sodium/nac/exceptions/io_exception.h"

#include <cerrno>
#include <string>

#include "sodium/nac/exceptions/exception.h"

namespace sodium {

IOException::IOException(ExceptionKind kind, const std::string &file_path) : Exception(kind) {
    message_ += " \'" + file_path + "\'";

    // if errno is set, get the errno string and append to message_
    if (errno != 0) {
        message_ += ": ";
        message_ += std::strerror(errno);
        message_ += "(errno " + std::to_string(errno) + ")";
    }
}

const char *IOException::what() const noexcept {
    return message_.c_str();
}

} // namespace sodium
