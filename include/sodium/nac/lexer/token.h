#ifndef SODIUM_NAC_LEXER_TOKEN_H
#define SODIUM_NAC_LEXER_TOKEN_H

#include <string_view>

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
     * @brief Construct a new Token object
     *
     * @param kind The kind of token being constructed, must be one of TokenKind.
     * @param position A pointer to the first character of the token.
     * @param length The length of the token.
     * @param line The line the token is on.
     * @param column The column the token is at.
     */
    Token(TokenKind kind, const char *position, int length, size_t line, size_t column);

    /**
     * @brief Get the kind of this token.
     *
     * @return TokenKind that is the kind of this token.
     */
    TokenKind kind() const noexcept;

    /**
     * @brief Get the line that this token is on.
     *
     * @return size_t that is the line number that this token is on.
     */
    size_t line() const noexcept;

    /**
     * @brief Get the column that this token is at.
     *
     * @return size_t that is the column number that this token is at.
     */
    size_t column() const noexcept;

    /**
     * @brief Get the position of this token.
     *
     * @return const char* to the first character of this token.
     */
    const char *position() const noexcept;

    /**
     * @brief Get the length of this token.
     *
     * @return int that is the length of this token.
     */
    int length() const noexcept;

    /**
     * @brief Get the value of this token.
     *
     * @return std::string_view that is the value of this token.
     */
    std::string_view value() const noexcept;

private:
    TokenKind kind_;

    size_t line_;
    size_t column_;

    const char *position_; // pointer to start of token
    int length_;
    std::string_view value_;
};

} // namespace sodium

#endif // SODIUM_NAC_LEXER_TOKEN_H
