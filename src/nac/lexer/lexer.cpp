#include "sodium/nac/lexer/lexer.h"

#include <cctype>

#include "sodium/nac/exceptions/exception.h"

nac::Lexer::Lexer(std::string_view string) : string_(string), index_(0) {}

std::vector<nac::Token> nac::Lexer::tokenize() {
    std::vector<nac::Token> tokens{};

    // skip any leading whitespace
    skipWhitespace();

    while (index_ < string_.size()) {
        nac::Token token = getNextToken();
        tokens.push_back(token);

        advance(token.value().size());
        skipWhitespace();
    }

    // add the EOF token to the end of the vector once end of string reached
    tokens.push_back(nac::Token(nac::TokenKind::TOKEN_EOF, ""));

    return tokens;
}

nac::Token nac::Lexer::getNextToken() {
    // if we have come across an identifier
    if (validIdentifierFirstCharacter(string_[index_])) {
        // extract the identifier from the string
        std::string identifier(string_.substr(index_, getIdentifierLength()));

        if (isKeyword(identifier)) {
            return nac::Token(nac::TokenKind::TOKEN_KEYWORD, identifier);
        }

        if (isType(identifier)) {
            return nac::Token(nac::TokenKind::TOKEN_TYPE, identifier);
        }

        return nac::Token(nac::TokenKind::TOKEN_IDENTIFIER, identifier);
    }

    // if we have come across a numeric literal
    if (isdigit(string_[index_])) {
        // extract the numeric literal from the string
        std::string numericLiteral(string_.substr(index_, getNumericLiteralLength()));
        return nac::Token(nac::TokenKind::TOKEN_NUMERIC_LITERAL, numericLiteral);
    }

    // otherwise
    switch (string_[index_]) {
        case ':': return nac::Token(nac::TokenKind::TOKEN_COLON, ":");
        case '{': return nac::Token(nac::TokenKind::TOKEN_LEFT_BRACE, "{");
        case '(': return nac::Token(nac::TokenKind::TOKEN_LEFT_PAREN, "(");
        case '}': return nac::Token(nac::TokenKind::TOKEN_RIGHT_BRACE, "}");
        case ')': return nac::Token(nac::TokenKind::TOKEN_RIGHT_PAREN, ")");
        case ';': return nac::Token(nac::TokenKind::TOKEN_SEMI_COLON, ";");
        default:
            std::string message = std::string("unexpected token \'\'");
            message.insert(message.size() - 1, 1, string_[index_]);
            throw nac::Exception(message);
    }
}

void nac::Lexer::advance(size_t offset) {
    index_ += offset;

    if (index_ > string_.size()) {
        index_ = string_.size();
    }
}

void nac::Lexer::skipWhitespace() {
    while (std::isspace(string_[index_])) {
        advance(1);
    }
}

size_t nac::Lexer::getIdentifierLength() {
    size_t end = index_;
    while (end < string_.size() && validIdentifierCharacter(string_[end])) {
        end++;
    }

    return end - index_;
}

size_t nac::Lexer::getNumericLiteralLength() {
    size_t end = index_;
    while (end < string_.size() && std::isdigit(string_[end])) {
        end++;
    }

    return end - index_;
}

inline bool nac::Lexer::validIdentifierFirstCharacter(char c) {
    return std::isalpha(c) || c == '_' || c == '$';
}

inline bool nac::Lexer::validIdentifierCharacter(char c) {
    return validIdentifierFirstCharacter(c) || std::isdigit(c);
}

inline bool nac::Lexer::isKeyword(const std::string &identifier) {
    return nac::KEYWORDS.find(identifier) != nac::KEYWORDS.end();
}

inline bool nac::Lexer::isType(const std::string &identifier) {
    return nac::TYPES.find(identifier) != nac::TYPES.end();
}
