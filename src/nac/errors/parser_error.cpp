#include "sodium/nac/errors/parser_error.h"

#include <string_view>

#include "sodium/nac/errors/error.h"
#include "sodium/nac/token/token.h"

namespace sodium {

ParserError::ParserError(ErrorKind error_kind, const Token &token, std::string_view message)
        : Error(error_kind, token.range().start()) {
    message_ += ": ";
    message_ += message;
    message_ += ", found \'" + token.value() + "\'";
}

} // namespace sodium
