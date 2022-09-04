#include "sodium/nac/lexer/token.h"

#include <string_view>

#include "sodium/nac/basic/source_location.h"

namespace sodium {

Token::Token(TokenKind kind, SourceLocation location, size_t length)
        : kind_(kind),
          location_(location),
          length_(length),
          value_(location_.position(), length_) {}

TokenKind Token::kind() const noexcept {
    return kind_;
}

SourceLocation Token::location() const noexcept {
    return location_;
}

size_t Token::length() const noexcept {
    return length_;
}

std::string_view Token::value() const noexcept {
    return value_;
}

} // namespace sodium
