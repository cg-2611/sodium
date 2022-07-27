#ifndef SODIUM_NAC_EXCEPTIONS_IO_EXCEPTION_H
#define SODIUM_NAC_EXCEPTIONS_IO_EXCEPTION_H

#include "exception.h"

namespace nac {

/**
 *  Class for I/O exceptions that occur in the Sodium compiler. Inherits nac::Exception.
 */
class IOException : public Exception {
public:
    /**
     * Constructor for IOException. It appends, to message_, the path to the file involved in the error and if errno
     * is set, then the errno string is appended also.
     * @param error The error for which the message is to be generated.
     * @param filePath The path to the file involved in the error.
     */
    IOException(Error error, const std::string& filePath);

    /**
     * Destructor for IOException.
     */
    ~IOException() = default;

    /**
     * @return The C string of the generated error message.
     */
    const char* what() const noexcept override;
};

} // namespace nac

#endif // SODIUM_NAC_EXCEPTIONS_IO_EXCEPTION_H
