#include "sodium/nac/lexer/token.h"

const std::set<std::string> sodium::nac::Token::KEYWORDS = {
    "func", "return"
};

const std::set<std::string> sodium::nac::Token::TYPES = {
    "int"
};

sodium::nac::Token::Token(sodium::nac::TokenKind tokenKind, const std::string tokenValue) {
    kind_ = tokenKind;
    value_ = tokenValue;
    next_ = nullptr;
}

sodium::nac::TokenKind sodium::nac::Token::getKind() const {
    return kind_;
}

const std::string sodium::nac::Token::getKindAsString() const {
    switch (kind_) {
        case sodium::nac::TokenKind::TOKEN_EOF: return "TOKEN_EOF";
        case sodium::nac::TokenKind::TOKEN_KEYWORD: return "TOKEN_KEYWORD";
        case sodium::nac::TokenKind::TOKEN_TYPE: return "TOKEN_TYPE";
        case sodium::nac::TokenKind::TOKEN_IDENTIFIER: return "TOKEN_IDENTIFIER";
        case sodium::nac::TokenKind::TOKEN_NUMERIC_LITERAL: return "TOKEN_NUMERIC_LITERAL";
        case sodium::nac::TokenKind::TOKEN_LEFT_PAREN: return "TOKEN_LEFT_PAREN";
        case sodium::nac::TokenKind::TOKEN_RIGHT_PAREN: return "TOKEN_RIGHT_PAREN";
        case sodium::nac::TokenKind::TOKEN_LEFT_BRACE: return "TOKEN_LEFT_BRACE";
        case sodium::nac::TokenKind::TOKEN_RIGHT_BRACE: return "TOKEN_RIGHT_BRACE";
        case sodium::nac::TokenKind::TOKEN_COLON: return "TOKEN_COLON";
        case sodium::nac::TokenKind::TOKEN_SEMI_COLON: return "TOKEN_SEMI_COLON";
        default: return "UNRECOGNIZED_TOKEN";
    };
}

const std::string sodium::nac::Token::getValue() const {
    return value_;
}

const std::shared_ptr<sodium::nac::Token> sodium::nac::Token::getNext() const {
    return next_;
}

void sodium::nac::Token::setNext(std::shared_ptr<Token> nextToken) {
    next_ = nextToken;
}
