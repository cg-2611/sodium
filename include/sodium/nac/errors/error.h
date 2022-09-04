#ifndef SODIUM_NAC_ERROR_ERROR_H
#define SODIUM_NAC_ERROR_ERROR_H

#include <string>

#include "sodium/nac/basic/source_location.h"

namespace sodium {

/**
 * @brief An enum of the different kinds of errors that can be generated.
 *
 */
enum class ErrorKind {
    SYNTAX_ERROR,
    UNRECOGNISED_TOKEN
};

/**
 * @brief A base class for errors generated in the Sodium compiler.
 *
 */
class Error {
public:
    /**
     * @brief Construct a new Error object.
     *
     * @param kind The kind of the error, must be one of ErrorKind.
     * @param location The location in the source at which the error has occurred.
     */
    Error(ErrorKind kind, SourceLocation location);

    /**
     * @brief Destroy the Error object.
     *
     */
    virtual ~Error() = default;

    /**
     * @brief Get the specific error message generated for this error.
     *
     * @return const std::string& that is the specific error message generated for this error.
     */
    const std::string &message() const;

protected:
    std::string message_;
};

} // namespace sodium

#endif // SODIUM_NAC_ERROR_ERROR_H
