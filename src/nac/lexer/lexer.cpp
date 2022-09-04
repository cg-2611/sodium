#include "sodium/nac/lexer/lexer.h"

#include <string_view>
#include <vector>

#include "sodium/nac/errors/error.h"
#include "sodium/nac/errors/error_manager.h"
#include "sodium/nac/errors/lexer_error.h"
#include "sodium/nac/lexer/token.h"

namespace sodium {

// helper functions
static inline bool isSpace(char c) noexcept;
static inline bool isDigit(char c) noexcept;
static inline bool isAlpha(char c) noexcept;
static inline bool isIdentifierCharacter(char c) noexcept;
static inline bool isReserved(const char *start, size_t length);

Lexer::Lexer(std::string_view src) : start_(src.data()), current_(start_), end_(src.end()), line_(1), column_(1) {}

std::vector<Token> Lexer::tokenize() {
    std::vector<Token> tokens{};
    while (!atEndOfString()) {
        Token currentToken(getNextToken());

        // if the token is an error, add it to the vector of errors and read the next token
        if (currentToken.kind() == TokenKind::ERROR_TOKEN) {
            continue;
        }

        tokens.push_back(currentToken);
    }

    return tokens;
}

Token Lexer::getNextToken() {
    Token token(nextToken());

    // skip error tokens to return next legal token
    while (token.kind() == TokenKind::ERROR_TOKEN) {
        token = nextToken();
    }

    return token;
}

Token Lexer::nextToken() {
    // skip leading whitespace before the token
    skipWhitespace();
    start_ = current_;

    if (atEndOfString()) {
        return makeToken(TokenKind::EOF_TOKEN);
    }

    advance();

    if (isIdentifierCharacter(*start_)) {
        // read an identifier if the current token starts with a valid identifier character
        size_t length = readIdentifier();

        if (isReserved(start_, length)) {
            return makeToken(RESERVED_WORDS.at(std::string_view(start_, length)));
        }

        return makeToken(TokenKind::IDENTIFIER);
    }

    if (isDigit(*start_)) {
        // read a numeric literal if the current token starts with a digit
        readNumericLiteral();
        return makeToken(TokenKind::NUMERIC_LITERAL);
    }

    switch (*start_) {
        case '{': return makeToken(TokenKind::LEFT_BRACE);
        case '}': return makeToken(TokenKind::RIGHT_BRACE);
        case '(': return makeToken(TokenKind::LEFT_PAREN);
        case ')': return makeToken(TokenKind::RIGHT_PAREN);
        case ';': return makeToken(TokenKind::SEMICOLON);
        case '-':
            // if the current token starts with a '-' and the next character is a '>'
            if (*current_ == '>') {
                advance();
                return makeToken(TokenKind::ARROW);
            }
            [[fallthrough]]; // temporary until '-' is a valid token
        default:
            Token errorToken = makeToken(TokenKind::ERROR_TOKEN);
            ErrorManager::addError<LexerError>(ErrorKind::UNRECOGNISED_TOKEN, errorToken);
            return errorToken;
    }
}

Token Lexer::makeToken(TokenKind kind) {
    return Token(kind, start_, current_ - start_, line_, column_);
}

size_t Lexer::readIdentifier() {
    while (isIdentifierCharacter(*current_) || isDigit(*current_)) {
        advance();
    }

    return current_ - start_;
}

size_t Lexer::readNumericLiteral() {
    while (isDigit(*current_)) {
        advance();
    }

    return current_ - start_;
}

void Lexer::advance() noexcept {
    ++current_;
    ++column_;
}

void Lexer::skipWhitespace() noexcept {
    while (isSpace(*current_)) {
        if (atEndOfLine()) {
            ++line_;
            column_ = 0;
        }
        advance();
    }
}

inline bool Lexer::atEndOfString() const noexcept {
    return start_ >= end_;
}

inline bool Lexer::atEndOfLine() const noexcept {
    return *current_ == '\n';
}

static inline bool isReserved(const char *start, size_t length) {
    return RESERVED_WORDS.contains(std::string_view(start, length));
}

static inline bool isIdentifierCharacter(char c) noexcept {
    return isAlpha(c) || c == '_';
}

static inline bool isAlpha(char c) noexcept {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

static inline bool isDigit(char c) noexcept {
    return c >= '0' && c <= '9';
}

static inline bool isSpace(char c) noexcept {
    return c == '\t' || c == '\v' || c == '\f' || c == '\r' || c == '\n' || c == ' ';
}

} // namespace sodium
