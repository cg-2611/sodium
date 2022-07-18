#include "sodium/nac/lexer/token.h"

nac::Token::Token(nac::TokenKind kind, const std::string &value) : kind_(kind), value_(value) {}

nac::TokenKind nac::Token::getKind() const {
    return kind_;
}

const std::string nac::Token::getKindAsString() const {
    switch (kind_) {
        case nac::TokenKind::TOKEN_EOF: return "TOKEN_EOF";
        case nac::TokenKind::TOKEN_KEYWORD: return "TOKEN_KEYWORD";
        case nac::TokenKind::TOKEN_TYPE: return "TOKEN_TYPE";
        case nac::TokenKind::TOKEN_IDENTIFIER: return "TOKEN_IDENTIFIER";
        case nac::TokenKind::TOKEN_NUMERIC_LITERAL: return "TOKEN_NUMERIC_LITERAL";
        case nac::TokenKind::TOKEN_LEFT_PAREN: return "TOKEN_LEFT_PAREN";
        case nac::TokenKind::TOKEN_RIGHT_PAREN: return "TOKEN_RIGHT_PAREN";
        case nac::TokenKind::TOKEN_LEFT_BRACE: return "TOKEN_LEFT_BRACE";
        case nac::TokenKind::TOKEN_RIGHT_BRACE: return "TOKEN_RIGHT_BRACE";
        case nac::TokenKind::TOKEN_COLON: return "TOKEN_COLON";
        case nac::TokenKind::TOKEN_SEMI_COLON: return "TOKEN_SEMI_COLON";
        default: return "UNRECOGNIZED_TOKEN";
    }
}

const std::string & nac::Token::getValue() const {
    return value_;
}
