#include "sodium/nac/errors/lexer_error.h"

#include "sodium/nac/errors/error.h"
#include "sodium/nac/lexer/token.h"
#include "sodium/nac/util/string_formatter.h"

namespace sodium {

LexerError::LexerError(ErrorKind errorKind, const Token &token) : Error(errorKind, token.location()) {
    message_ += StringFormatter::formatString(" \'%.*s\'", token.length(), token.value().data());
}

} // namespace sodium
