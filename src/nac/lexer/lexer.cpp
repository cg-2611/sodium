#include "sodium/nac/lexer/lexer.h"

#include <cctype>

#include "sodium/nac/exceptions/lexer_exception.h"

namespace nac {

static constexpr bool isValidIdentifierCharacter(char c);    // returns true if c is a valid identifier character
static inline bool isKeyword(const std::string& identifier); // returns true if identifier is a keyword
static inline bool isType(const std::string& identifier);    // returns true if identifier is a type

Lexer::Lexer(std::string_view string) : string_(string), index_(0) {}

[[nodiscard]] std::unique_ptr<Token> Lexer::tokenize() {
    // skip leading whitespace
    skipWhitespace();

    std::unique_ptr<Token> eofToken = std::make_unique<Token>(TokenKind::TOKEN_EOF, "");

    // return eof token for empty string
    if (index_ >= string_.size()) {
        return eofToken;
    }

    // get the first token
    std::unique_ptr<Token> token = getNextToken();
    Token* currentToken = token.get();

    // advance the lexer to the end of the token
    advance(currentToken->value().size());
    skipWhitespace();

    // get the remaining tokens
    while (index_ < string_.size()) {
        currentToken->next(getNextToken());
        currentToken = currentToken->next();

        // advance the lexer to the end of the token
        advance(currentToken->value().size());
        skipWhitespace();
    }

    // end the token list with an eof token
    currentToken->next(std::move(eofToken));

    return token;
}

std::unique_ptr<Token> Lexer::getNextToken() {
    // if we have come across an identifier
    if (isValidIdentifierCharacter(string_[index_])) {
        // extract the identifier from the string
        std::string identifier(string_.substr(index_, getIdentifierLength()));

        if (isKeyword(identifier)) {
            return std::make_unique<Token>(TokenKind::TOKEN_KEYWORD, identifier);
        }

        if (isType(identifier)) {
            return std::make_unique<Token>(TokenKind::TOKEN_TYPE, identifier);
        }

        return std::make_unique<Token>(TokenKind::TOKEN_IDENTIFIER, identifier);
    }

    // if we have come across a numeric literal
    if (isdigit(string_[index_])) {
        // extract the numeric literal from the string
        std::string numericLiteral(string_.substr(index_, getNumericLiteralLength()));
        return std::make_unique<Token>(TokenKind::TOKEN_NUMERIC_LITERAL, numericLiteral);
    }

    // otherwise
    switch (string_[index_]) {
        case '-':
            if (peek() == '>') {
                return std::make_unique<Token>(TokenKind::TOKEN_RETURN_ARROW, "->");
            }
            // temporary until '-' is a valid token
            throw LexerException(Error::UNRECOGNISED_TOKEN, string_[index_]);
        case '{': return std::make_unique<Token>(TokenKind::TOKEN_LEFT_BRACE, "{");
        case '(': return std::make_unique<Token>(TokenKind::TOKEN_LEFT_PAREN, "(");
        case '}': return std::make_unique<Token>(TokenKind::TOKEN_RIGHT_BRACE, "}");
        case ')': return std::make_unique<Token>(TokenKind::TOKEN_RIGHT_PAREN, ")");
        case ';': return std::make_unique<Token>(TokenKind::TOKEN_SEMI_COLON, ";");
        default: throw LexerException(Error::UNRECOGNISED_TOKEN, string_[index_]);
    }
}

void Lexer::advance(size_t offset) {
    index_ += offset;

    if (index_ > string_.size()) {
        index_ = string_.size();
    }
}

void Lexer::skipWhitespace() {
    while (std::isspace(string_[index_])) {
        advance(1);
    }
}

constexpr char Lexer::peek() {
    return string_[index_ + 1];
}

constexpr size_t Lexer::getIdentifierLength() {
    size_t end = index_;
    while (end < string_.size() && (isValidIdentifierCharacter(string_[end]) || std::isdigit(string_[end]))) {
        ++end;
    }

    return end - index_;
}

constexpr size_t Lexer::getNumericLiteralLength() {
    size_t end = index_;
    while (end < string_.size() && std::isdigit(string_[end])) {
        ++end;
    }

    return end - index_;
}

static constexpr bool isValidIdentifierCharacter(char c) {
    return std::isalpha(c) || c == '_';
}

static inline bool isKeyword(const std::string& identifier) {
    return KEYWORDS.find(identifier) != KEYWORDS.end();
}

static inline bool isType(const std::string& identifier) {
    return TYPES.find(identifier) != TYPES.end();
}

} // namespace nac
