#include "sodium/nac/exceptions/io_exception.h"

#include <cerrno>
#include <string>

#include "sodium/nac/exceptions/exception.h"
#include "sodium/nac/util/string_formatter.h"

namespace sodium {

IOException::IOException(ExceptionKind kind, const std::string &file_path) : Exception(kind) {
    message_ += StringFormatter::format_string(" \'%s\'", file_path.c_str());

    // if errno is set, get the errno string and append to message_
    if (errno != 0) {
        message_ += StringFormatter::format_string(": %s (errno %d)", std::strerror(errno), errno);
    }
}

const char *IOException::what() const noexcept {
    return message_.c_str();
}

} // namespace sodium
