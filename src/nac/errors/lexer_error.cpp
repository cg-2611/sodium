#include "sodium/nac/errors/lexer_error.h"

#include "sodium/nac/errors/error.h"
#include "sodium/nac/token/token.h"
#include "sodium/nac/util/string_formatter.h"

namespace sodium {

LexerError::LexerError(ErrorKind error_kind, const Token &token) : Error(error_kind, token.location()) {
    message_ += StringFormatter::format_string(" \'%.*s\'", token.length(), token.value().data());
}

} // namespace sodium
