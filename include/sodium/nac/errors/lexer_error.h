#ifndef SODIUM_NAC_ERROR_LEXER_ERROR_H
#define SODIUM_NAC_ERROR_LEXER_ERROR_H

#include "sodium/nac/errors/error.h"
#include "sodium/nac/lexer/token.h"

namespace nac {

/**
 * A derived class of nac::Error for errors generated in the lexer.
 */
class LexerError : public Error {
public:
    /**
     * Constructor for LexerError.
     * @param kind The kind of the error, must be one of nac::ErrorKind.
     * @param token The token that has caused the error.
     */
    LexerError(ErrorKind kind, Token *token);

    /**
     * Destructor for LexerError.
     */
    ~LexerError() = default;
};

} // namespace nac

#endif // SODIUM_NAC_ERROR_LEXER_ERROR_H
