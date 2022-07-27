#include "sodium/nac/lexer/token.h"

#include <utility>

namespace nac {

Token::Token(TokenKind kind, std::string  value) : kind_(kind), value_(std::move(value)), next_(nullptr) {}

TokenKind Token::kind() const noexcept {
    return kind_;
}

const std::string& Token::value() const noexcept {
    return value_;
}

Token* Token::next() const noexcept {
    return next_.get();
}

void Token::next(std::unique_ptr<Token> next) noexcept {
    next_ = std::move(next);
}

} // namespace nac
