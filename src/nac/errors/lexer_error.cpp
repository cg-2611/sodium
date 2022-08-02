#include "sodium/nac/errors/lexer_error.h"

#include <cstdio>

#include "sodium/nac/errors/error.h"
#include "sodium/nac/lexer/token.h"

namespace nac {

LexerError::LexerError(ErrorKind errorKind, Token *token) : Error(errorKind, token->line(), token->column()) {
    const size_t mLength = token->length() + 3;
    std::string m(mLength, '\0');
    std::sprintf(m.data(), " \'%.*s\'", token->length(), token->value().data());

    message_ += m;
}

} // namespace nac
