#include "sodium/nac/errors/lexer_error.h"

#include "sodium/nac/errors/error.h"
#include "sodium/nac/token/token.h"

namespace sodium {

LexerError::LexerError(ErrorKind error_kind, const Token &token) : Error(error_kind, token.range().start()) {
    message_ += " \'" + token.value() + "\'";
}

} // namespace sodium
