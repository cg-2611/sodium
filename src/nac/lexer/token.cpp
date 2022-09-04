#include "sodium/nac/lexer/token.h"

#include <string_view>

namespace sodium {

Token::Token(TokenKind kind, const char *position, int length, size_t line, size_t column)
        : kind_(kind),
          line_(line),
          column_(column - length),
          position_(position),
          length_(length),
          value_(position_, length_) {}

TokenKind Token::kind() const noexcept {
    return kind_;
}

size_t Token::line() const noexcept {
    return line_;
}

size_t Token::column() const noexcept {
    return column_;
}

const char *Token::position() const noexcept {
    return position_;
}

int Token::length() const noexcept {
    return length_;
}

std::string_view Token::value() const noexcept {
    return value_;
}

} // namespace sodium
