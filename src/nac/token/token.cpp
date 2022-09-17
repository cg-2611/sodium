#include "sodium/nac/token/token.h"

#include <string>

#include "sodium/nac/basic/source_range.h"
#include "sodium/nac/token/token_kind.h"

namespace sodium {

Token::Token(TokenKind kind, SourceRange range, int length) : kind_(kind), range_(range), length_(length) {}

TokenKind Token::kind() const {
    return kind_;
}

SourceRange Token::range() const {
    return range_;
}

int Token::length() const {
    return length_;
}

std::string Token::value() const {
    return std::string(range_.start().position(), length_);
}

Token Token::dummy() {
    return Token(TokenKind::EOF_TOKEN, SourceRange(), 0);
}

} // namespace sodium
