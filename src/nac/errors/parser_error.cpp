#include "sodium/nac/errors/parser_error.h"

#include <string_view>

#include "sodium/nac/errors/error.h"
#include "sodium/nac/lexer/token.h"
#include "sodium/nac/util/string_formatter.h"

namespace sodium {

ParserError::ParserError(ErrorKind errorKind, const Token &token, std::string_view message)
        : Error(errorKind, token.line(), token.column()) {
    message_ += StringFormatter::formatString(": %.*s", message.size(), message.data());
    message_ += StringFormatter::formatString(", found \'%.*s\'", token.length(), token.value().data());
}

} // namespace sodium
