#include "sodium/nac/lexer/lexer_diagnostics.h"

#include <string>
#include <string_view>

#include "sodium/nac/diagnostics/error.h"
#include "sodium/nac/token/token.h"

namespace sodium {

LexerError::LexerError(LexerErrorKind kind, const Token &token)
        : Error(ErrorKind::LEXER, token.range().start()),
          kind_(kind) {
    message_ += Error::message() + " ";
    message_ += description_from_kind(kind_);
    message_ += " \'" + token.value() + "\'";
}

LexerErrorKind LexerError::kind() const {
    return kind_;
}

const std::string &LexerError::message() const {
    return message_;
}

constexpr std::string_view LexerError::description_from_kind(LexerErrorKind kind) {
    switch (kind) {
        case LexerErrorKind::UNRECOGNISED_TOKEN: return "unrecognised token";
        default: "unkown lexer error";
    }
}

} // namespace sodium
