#ifndef SODIUM_NAC_EXCEPTIONS_EXCEPTION_H
#define SODIUM_NAC_EXCEPTIONS_EXCEPTION_H

#include <exception>
#include <string>

namespace sodium {

/**
 * @brief An enum of the different kinds of exception that can be thrown.
 *
 */
enum class ExceptionKind {
    ERRORS_GENERATED,
    FILE_OPEN_FAIL,
    FILE_READ_FAIL
};

/**
 * @brief Base class for Sodium compiler exceptions. Inherits std::exception.
 *
 */
class Exception : public std::exception {
public:
    /**
     * @brief Construct a new Exception object.
     *
     * @param kind The kind of the exception, must be one of ExceptionKind.
     */
    Exception(ExceptionKind kind);

    /**
     * @brief Destroy the Exception object.
     *
     */
    virtual ~Exception() = default;

    /**
     * @brief Inherited from std::exception. Overriden in derived classes.
     *
     * @return const char* that is the exception message.
     */
    virtual const char *what() const noexcept;

protected:
    std::string message_; // the string to be output when what() is called
};

} // namespace sodium

#endif // SODIUM_NAC_EXCEPTIONS_EXCEPTION_H
