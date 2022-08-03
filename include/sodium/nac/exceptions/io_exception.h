#ifndef SODIUM_NAC_EXCEPTIONS_IO_EXCEPTION_H
#define SODIUM_NAC_EXCEPTIONS_IO_EXCEPTION_H

#include <string>

#include "sodium/nac/exceptions/exception.h"

namespace nac {

/**
 * A derived class of nac::Exception for exceptions generated during file i/o.
 */
class IOException : public Exception {
public:
    /**
     * Constructor for IOException. If errno is set, the errno string is appended to the exception message.
     * @param kind The kind of the exception, must be one of nac::ExceptionKind.
     * @param filePath The path of the file that has caused the exception to be thrown.
     */
    IOException(ExceptionKind kind, const std::string &filePath);

    /**
     * Destructor for IOException.
     */
    ~IOException() = default;

    /**
     * @return The exception message.
     */
    const char *what() const noexcept override;
};

} // namespace nac

#endif // SODIUM_NAC_EXCEPTIONS_IO_EXCEPTION_H
