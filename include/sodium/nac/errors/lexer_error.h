#ifndef SODIUM_NAC_ERROR_LEXER_ERROR_H
#define SODIUM_NAC_ERROR_LEXER_ERROR_H

#include "sodium/nac/errors/error.h"
#include "sodium/nac/lexer/token.h"

namespace sodium {

/**
 * @brief A derived class of sodium::Error for errors generated in the lexer.
 *
 */
class LexerError : public Error {
public:
    /**
     * @brief Construct a new Lexer Error object
     *
     * @param kind The kind of the error, must be one of ErrorKind.
     * @param token The token that has caused the error.
     */
    LexerError(ErrorKind kind, Token *token);

    /**
     * @brief Destroy the Lexer Error object.
     *
     */
    ~LexerError() = default;
};

} // namespace sodium

#endif // SODIUM_NAC_ERROR_LEXER_ERROR_H
