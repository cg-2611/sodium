#include "sodium/nac/exceptions/io_exception.h"

#include <cerrno>
#include <cstdio>
#include <string>

#include "sodium/nac/exceptions/exception.h"

namespace nac {

IOException::IOException(ExceptionKind exceptionKind, const std::string &filePath) : Exception(exceptionKind) {
    size_t mLength = filePath.size() + 3;
    std::string m(mLength, '\0');
    std::sprintf(m.data(), " \'%s\'", filePath.c_str());
    message_ += m;

    // if errno is set, get the errno string and append to message_
    if (errno != 0) {
        std::string errnoMessage(std::strerror(errno));
        const size_t nLength = errnoMessage.size() + std::to_string(errno).size() + 11;
        std::string n(nLength, '\0');
        std::sprintf(n.data(), ": %s (errno %d)", errnoMessage.c_str(), errno);
        message_ += n;
    }
}

const char *IOException::what() const noexcept {
    return message_.c_str();
}

} // namespace nac
