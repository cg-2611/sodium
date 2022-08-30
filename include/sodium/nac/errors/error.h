#ifndef SODIUM_NAC_ERROR_ERROR_H
#define SODIUM_NAC_ERROR_ERROR_H

#include <string>

namespace sodium {

/**
 * An enum of the different kinds of errors that can be generated.
 */
enum class ErrorKind {
    SYNTAX_ERROR,
    UNRECOGNISED_TOKEN
};

/**
 * A base class for errors generated in the Sodium compiler.
 */
class Error {
public:
    /**
     * Constructor for Error.
     * @param kind The kind of the error, must be one of sodium::ErrorKind.
     * @param line The line at which the error has occurred.
     * @param column The column at which the error has occurred.
     */
    Error(ErrorKind kind, size_t line, size_t column);

    /**
     * Destructor for Error.
     */
    virtual ~Error() = default;

    /**
     * @return The specific message generated for this error.
     */
    const std::string &message() const;

protected:
    std::string message_;
};

} // namespace sodium

#endif // SODIUM_NAC_ERROR_ERROR_H
