#ifndef SODIUM_NAC_LEXER_TOKEN_H
#define SODIUM_NAC_LEXER_TOKEN_H

#include <memory>
#include <string_view>

namespace sodium {

/**
 * An enum of the different kinds of tokens currently used in Sodium.
 */
enum class TokenKind {
    // reserved tokens
    KEYWORD,
    TYPE,

    // literal tokens
    IDENTIFIER,
    NUMERIC_LITERAL,

    // one character tokens
    LEFT_BRACE,
    RIGHT_BRACE,
    LEFT_PAREN,
    RIGHT_PAREN,

    // two character tokens
    ARROW,

    // miscellaneous tokens
    EOF_TOKEN,
    EOL_TOKEN,
    ERROR_TOKEN
};

/**
 * A Sodium programming language token.
 */
class Token {
public:
    /**
     * Constructor for a token.
     * @param kind The kind of token being constructed, must be one of sodium::TokenKind.
     * @param position A pointer to the first character of the token.
     * @param length The length of the token.
     * @param line The line the token is on.
     * @param column The column the token is at.
     */
    Token(TokenKind kind, const char *position, size_t length, size_t line, size_t column);

    /// Explicitly delete copy constructor as unique_ptr is non-copyable.
    Token(const Token &other) = delete;

    /**
     * Default move constructor.
     * @param other The token of which to move the resources from into this token.
     */
    Token(Token &&other) noexcept = default;

    /**
     * Destructor for Token.
     */
    ~Token() = default;

    /// Explicitly delete copy assignment operator as unique_ptr is non-copyable.
    Token &operator=(const Token &other) = delete;

    /**
     * Default move assignment operator.
     * @param other The token which is being move assigned to this token.
     */
    Token &operator=(Token &&other) noexcept = default;

    /**
     * @return The kind of this token.
     */
    TokenKind kind() const noexcept;

    /**
     * @return The line the token is on.
     */
    size_t line() const noexcept;

    /**
     * @return The column the token is at.
     */
    size_t column() const noexcept;

    /**
     * @return A pointer to the first character of the token.
     */
    const char *position() const noexcept;

    /**
     * @return The length of the token.
     */
    int length() const noexcept;

    /**
     * @return The value of this token as a string.
     */
    std::string_view value() const noexcept;

    /**
     * @return A raw pointer to the token that comes after this token.
     */
    Token *next() const noexcept;

    /**
     * Modifier method for the next token.
     * @param next An std::unique_ptr<Token> which is moved into the next_ member of this token.
     */
    void next(std::unique_ptr<Token> next) noexcept;

private:
    TokenKind kind_;

    size_t line_;
    size_t column_;

    const char *position_; // pointer to start of token
    int length_;
    std::string_view value_;

    std::unique_ptr<Token> next_;
};

} // namespace sodium

#endif // SODIUM_NAC_LEXER_TOKEN_H
