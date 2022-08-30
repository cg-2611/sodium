#include "sodium/nac/lexer/lexer.h"

#include <memory>
#include <string_view>
#include <utility>

#include "sodium/nac/errors/error_manager.h"
#include "sodium/nac/errors/error.h"
#include "sodium/nac/errors/lexer_error.h"
#include "sodium/nac/lexer/token.h"

namespace sodium {

// helper functions
static constexpr bool isSpace(char c) noexcept;
static constexpr bool isDigit(char c) noexcept;
static constexpr bool isAlpha(char c) noexcept;
static constexpr bool isIdentifierCharacter(char c) noexcept;
static inline bool isKeyword(const char *start, size_t length); // returns true if identifier is a keyword
static inline bool isType(const char *start, size_t length);    // returns true if identifier is a type

Lexer::Lexer(std::string_view src) : start_(src.data()), current_(start_), end_(src.end()), line_(1), column_(1) {}

[[nodiscard]] std::unique_ptr<Token> Lexer::tokenize() {
    std::unique_ptr<Token> token(getNextToken());

    // skip all tokens until the first valid token is encountered
    while (token->kind() == TokenKind::ERROR_TOKEN) {
        // for each error token encountered add it to the vector of errors
        ErrorManager::addError<LexerError>(ErrorKind::UNRECOGNISED_TOKEN, token.get());
        token = getNextToken();
    }

    Token *currentToken = token.get();

    while (!atEndOfString()) {
        currentToken->next(getNextToken());

        // if the token is an error, add it to the vector of errors and read the next token
        if (currentToken->next()->kind() == TokenKind::ERROR_TOKEN) {
            ErrorManager::addError<LexerError>(ErrorKind::UNRECOGNISED_TOKEN, currentToken->next());
            continue;
        }

        currentToken = currentToken->next();
    }

    return token;
}

[[nodiscard]] std::unique_ptr<Token> Lexer::getNextToken() {
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

        if (isKeyword(start_, length)) {
            return makeToken(TokenKind::KEYWORD);
        }

        if (isType(start_, length)) {
            return makeToken(TokenKind::TYPE);
        }

        return makeToken(TokenKind::IDENTIFIER);
    } else if (isDigit(*start_)) {
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
        default: return makeToken(TokenKind::ERROR_TOKEN);
    }
}

[[nodiscard]] std::unique_ptr<Token> Lexer::makeToken(TokenKind kind) {
    return std::make_unique<Token>(kind, start_, current_ - start_, line_, column_);
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

static inline bool isKeyword(const char *start, size_t length) {
    return KEYWORDS.contains(std::string_view(start, length));
}

static inline bool isType(const char *start, size_t length) {
    return TYPES.contains(std::string_view(start, length));
}

static constexpr bool isIdentifierCharacter(char c) noexcept {
    return isAlpha(c) || c == '_';
}

static constexpr bool isAlpha(char c) noexcept {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

static constexpr bool isDigit(char c) noexcept {
    return c >= '0' && c <= '9';
}

static constexpr bool isSpace(char c) noexcept {
    return c == '\t' || c == '\v' || c == '\f' || c == '\r' || c == '\n' || c == ' ';
}

} // namespace sodium
