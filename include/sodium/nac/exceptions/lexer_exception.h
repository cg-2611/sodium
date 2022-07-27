#ifndef SODIUM_NAC_EXCEPTIONS_LEXER_EXCEPTION_H
#define SODIUM_NAC_EXCEPTIONS_LEXER_EXCEPTION_H

#include "exception.h"

namespace nac {

/**
 *  Class for exceptions that occur in the Sodium compiler lexer. Inherits nac::Exception.
 */
class LexerException : public Exception {
public:
    /**
     * Constructor for LexerException. It appends, to message_, the character that has caused the exception.
     * @param error The error for which the message is to be generated.
     * @param token The character that has caused the exception.
     */
    LexerException(Error error, char token);

    /**
     * Destructor for LexerException.
     */
    ~LexerException() = default;

    /**
     * @return The C string of the generated error message.
     */
    const char* what() const noexcept override;
};

} // namespace nac

#endif // SODIUM_NAC_EXCEPTIONS_LEXER_EXCEPTION_H
