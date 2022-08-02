#include "sodium/nac/lexer/token.h"

#include <memory>
#include <string_view>
#include <utility>

namespace nac {

Token::Token(TokenKind kind, const char *position, size_t length, size_t line, size_t column)
        : kind_(kind),
          position_(position),
          length_(length),
          line_(line),
          column_(column - length),
          value_(position_, length_),
          next_(nullptr) {}

TokenKind Token::kind() const noexcept {
    return kind_;
}

std::string_view Token::value() const noexcept {
    return value_;
}

size_t Token::line() const noexcept{
    return line_;
}

size_t Token::column() const noexcept{
    return column_;
}

const char *Token::position() const noexcept {
    return position_;
}

int Token::length() const noexcept {
    return length_;
}

Token *Token::next() const noexcept {
    return next_.get();
}

void Token::next(std::unique_ptr<Token> next) noexcept {
    next_ = std::move(next);
}

} // namespace nac
