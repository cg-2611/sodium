#include "sodium/nac/token/token.h"

#include <string>

#include "sodium/nac/basic/source_range.h"

namespace sodium {

Token::Token(TokenKind kind, SourceRange range, uint32_t length) : kind_(kind), range_(range), length_(length) {}

TokenKind Token::kind() const {
    return kind_;
}

SourceRange Token::range() const {
    return range_;
}

uint32_t Token::length() const {
    return length_;
}

std::string Token::value() const {
    return std::string(range_.start().position(), length_);
}

Token Token::dummy() {
    return Token(TokenKind::EOF_TOKEN, SourceRange(), 0);
}

} // namespace sodium
