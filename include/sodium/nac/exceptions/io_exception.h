#ifndef SODIUM_NAC_EXCEPTIONS_IO_EXCEPTION_H
#define SODIUM_NAC_EXCEPTIONS_IO_EXCEPTION_H

#include <string>

#include "sodium/nac/exceptions/exception.h"

namespace sodium {

/**
 * @brief A derived class of sodium::Exception for exceptions generated during file i/o.
 *
 */
class IOException : public Exception {
public:
    /**
     * @brief Construct a new IOException object. If errno is set, the errno string is appended to the exception
     *        message.
     *
     * @param kind The kind of the exception, must be one of ExceptionKind.
     * @param filePath The path of the file that has caused the exception to be thrown.
     */
    IOException(ExceptionKind kind, const std::string &filePath);

    /**
     * @brief Destroy the IOException object.
     *
     */
    ~IOException() = default;

    /**
     * @brief Get the generated message for this exception.
     *
     * @return const char* that is the message for this exception.
     */
    const char *what() const noexcept override;
};

} // namespace sodium

#endif // SODIUM_NAC_EXCEPTIONS_IO_EXCEPTION_H
