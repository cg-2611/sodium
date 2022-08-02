#ifndef SODIUM_NAC_EXCEPTIONS_EXCEPTION_H
#define SODIUM_NAC_EXCEPTIONS_EXCEPTION_H

#include <exception>
#include <string>

namespace nac {

enum class ExceptionKind {
    ERRORS_GENERATED,
    FILE_OPEN_FAIL,
    FILE_READ_FAIL
};

class Exception : public std::exception {
public:
    Exception(ExceptionKind kind);
    virtual ~Exception() = default;

    virtual const char *what() const noexcept;

protected:
    std::string message_;
};

} // namespace nac

#endif // SODIUM_NAC_EXCEPTIONS_EXCEPTION_H
