#ifndef SODIUM_NAC_ERROR_LEXER_ERROR_H
#define SODIUM_NAC_ERROR_LEXER_ERROR_H

#include "sodium/nac/errors/error.h"
#include "sodium/nac/lexer/token.h"

namespace nac {

class LexerError : public Error {
public:
    LexerError(ErrorKind errorKind, Token *token);
    ~LexerError() = default;
};

} // namespace nac

#endif // SODIUM_NAC_ERROR_LEXER_ERROR_H
