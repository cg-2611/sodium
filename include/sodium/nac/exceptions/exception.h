#ifndef SODIUM_NAC_EXCEPTIONS_EXCEPTION_H
#define SODIUM_NAC_EXCEPTIONS_EXCEPTION_H

#include <exception>
#include <string>

namespace nac {

enum class Error {
    FILE_OPEN_FAIL,
    FILE_READ_FAIL,
    UNRECOGNISED_TOKEN
};

/**
 * Base class for Sodium compiler exceptions. Inherits std::exception.
 */
class Exception : public std::exception {
public:
    /**
     * Constructor for Exception. Data member message_ is initialized and an error message is appended to it.
     * @param error The error for which the message is to be generated.
     */
    Exception(const Error error);

    /**
     * Destructor for Exception.
     */
    virtual ~Exception() = default;

    /**
     * Inherited from std::exception. Overriden in derived classes.
     */
    virtual const char* what() const noexcept = 0;

protected:
    std::string message_; // the error message to be output when what() is called

private:
    // returns a message describing the argument error
    static constexpr std::string_view getErrorMessage(Error error);
};

} // namespace nac

#endif // SODIUM_NAC_EXCEPTIONS_EXCEPTION_H
