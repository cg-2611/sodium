#include "sodium/nac/exceptions/io_exception.h"

#include <cerrno>
#include <cstdio>
#include <string>

#include "sodium/nac/exceptions/exception.h"
#include "sodium/nac/util/string_formatter.h"

namespace nac {

IOException::IOException(ExceptionKind kind, const std::string &filePath) : Exception(kind) {
    message_ += StringFormatter::formatString(" \'%s\'", filePath.c_str());

    // if errno is set, get the errno string and append to message_
    if (errno != 0) {
        message_ += StringFormatter::formatString(": %s (errno %d)", std::strerror(errno), errno);
    }
}

const char *IOException::what() const noexcept {
    return message_.c_str();
}

} // namespace nac
