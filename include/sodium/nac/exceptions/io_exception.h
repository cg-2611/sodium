#ifndef SODIUM_NAC_EXCEPTIONS_IO_EXCEPTION_H
#define SODIUM_NAC_EXCEPTIONS_IO_EXCEPTION_H

#include <string>

#include "sodium/nac/exceptions/exception.h"

namespace nac {

class IOException : public Exception {
public:
    IOException(ExceptionKind exceptionKind, const std::string &filePath);
    ~IOException() = default;

    const char *what() const noexcept override;
};

} // namespace nac

#endif // SODIUM_NAC_EXCEPTIONS_IO_EXCEPTION_H
