#include "sodium/nac/errors/lexer_error.h"

#include "sodium/nac/errors/error.h"
#include "sodium/nac/lexer/token.h"
#include "sodium/nac/util/string_formatter.h"

namespace nac {

LexerError::LexerError(ErrorKind errorKind, Token *token) : Error(errorKind, token->line(), token->column()) {
    message_ += StringFormatter::formatString(" \'%.*s\'", token->length(), token->value().data());
}

} // namespace nac
