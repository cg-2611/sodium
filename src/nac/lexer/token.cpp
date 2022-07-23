#include "sodium/nac/lexer/token.h"

#include <utility>

namespace nac {

Token::Token(TokenKind kind, const std::string& value) : kind_(kind), value_(value), next_(nullptr) {}

TokenKind Token::kind() const {
    return kind_;
}

const std::string Token::kindString() const {
    switch (kind_) {
        case TokenKind::TOKEN_EOF: return "TOKEN_EOF";
        case TokenKind::TOKEN_KEYWORD: return "TOKEN_KEYWORD";
        case TokenKind::TOKEN_TYPE: return "TOKEN_TYPE";
        case TokenKind::TOKEN_IDENTIFIER: return "TOKEN_IDENTIFIER";
        case TokenKind::TOKEN_NUMERIC_LITERAL: return "TOKEN_NUMERIC_LITERAL";
        case TokenKind::TOKEN_COLON: return "TOKEN_COLON";
        case TokenKind::TOKEN_LEFT_BRACE: return "TOKEN_LEFT_BRACE";
        case TokenKind::TOKEN_LEFT_PAREN: return "TOKEN_LEFT_PAREN";
        case TokenKind::TOKEN_RIGHT_BRACE: return "TOKEN_RIGHT_BRACE";
        case TokenKind::TOKEN_RIGHT_PAREN: return "TOKEN_RIGHT_PAREN";
        case TokenKind::TOKEN_SEMI_COLON: return "TOKEN_SEMI_COLON";
        default: return "UNRECOGNIZED_TOKEN";
    }
}

const std::string& Token::value() const {
    return value_;
}

Token* Token::next() const {
    return next_.get();
}

void Token::next(std::unique_ptr<Token> next) {
    next_ = std::move(next);
}

} // namespace nac
