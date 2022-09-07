#include "sodium/nac/errors/parser_error.h"

#include <string_view>

#include "sodium/nac/errors/error.h"
#include "sodium/nac/token/token.h"
#include "sodium/nac/util/string_formatter.h"

namespace sodium {

ParserError::ParserError(ErrorKind error_kind, const Token &token, std::string_view message)
        : Error(error_kind, token.location()) {
    message_ += StringFormatter::format_string(": %.*s", message.size(), message.data());
    message_ += StringFormatter::format_string(", found \'%.*s\'", token.length(), token.value().data());
}

} // namespace sodium
