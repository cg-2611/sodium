#ifndef SODIUM_NAC_LEXER_TOKEN_H
#define SODIUM_NAC_LEXER_TOKEN_H

#include <string_view>

#include "sodium/nac/basic/source_location.h"

namespace sodium {

/**
 * @brief An enum of the different kinds of tokens currently used in Sodium.
 *
 */
enum class TokenKind {
    // reserved tokens
    KEYWORD_FUNC,
    KEYWORD_RETURN,
    TYPE,

    // literal tokens
    IDENTIFIER,
    NUMERIC_LITERAL,

    // one character tokens
    LEFT_BRACE,
    RIGHT_BRACE,
    LEFT_PAREN,
    RIGHT_PAREN,
    SEMICOLON,

    // two character tokens
    ARROW,

    // miscellaneous tokens
    EOF_TOKEN,
    ERROR_TOKEN
};

/**
 * @brief A Sodium programming language token.
 *
 */
class Token {
public:
    /**
     * @brief Construct a new Token object.
     *
     * @param kind The kind of token being constructed, must be one of TokenKind.
     * @param location The SourceLocation of the token.
     * @param length The length of the token.
     */
    Token(TokenKind kind, SourceLocation location, size_t length);

    /**
     * @brief Get the kind of this token.
     *
     * @return TokenKind that is the kind of this token.
     */
    TokenKind kind() const noexcept;

    /**
     * @brief Get the SourceLocation of this token.
     *
     * @return SourceLocation that is the location of this token.
     */
    SourceLocation location() const noexcept;

    /**
     * @brief Get the length of this token.
     *
     * @return int that is the length of this token.
     */
    size_t length() const noexcept;

    /**
     * @brief Get the value of this token.
     *
     * @return std::string_view that is the value of this token.
     */
    std::string_view value() const noexcept;

private:
    TokenKind kind_;
    SourceLocation location_;

    size_t length_;
    std::string_view value_;
};

} // namespace sodium

#endif // SODIUM_NAC_LEXER_TOKEN_H
