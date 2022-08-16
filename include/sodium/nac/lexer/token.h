#ifndef SODIUM_NAC_LEXER_TOKEN_H
#define SODIUM_NAC_LEXER_TOKEN_H

#include <memory>
#include <string_view>

namespace sodium {

/**
 * @brief An enum of the different kinds of tokens currently used in Sodium.
 *
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
    Token(TokenKind kind, const char *position, size_t length, size_t line, size_t column);

    /**
     * @brief Construct a new Token object. Default move constructor.
     *
     * @param other The token of which to move the resources from into this token.
     */
    Token(Token &&other) noexcept = default;

    /**
     * @brief Destroy the Token object.
     *
     */
    ~Token() = default;

    /**
     * @brief Default move assignment operator.
     *
     * @param other The token which is being move assigned to this token.
     * @return Token& that is this token after assignment.
     */
    Token &operator=(Token &&other) noexcept = default;

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

    /**
     * @brief Get the token that comes after this token.
     *
     * @return Token* to the token that comes after this token or nullptr if it is the last token.
     */
    Token *next() const noexcept;

    /**
     * @brief Set the token that comes after this token.
     *
     * @param next An std::unique_ptr<Token> which this token will point to as the token that comes after it.
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

    // deleted, private copy constructor so that a token cannot be copied as unique_ptr is non-copyable
    Token(const Token &other) = delete;

    // deleted, private copy assignment operator so that a token cannot be copied as unique_ptr is non-copyable
    Token &operator=(const Token &other) = delete;
};

} // namespace sodium

#endif // SODIUM_NAC_LEXER_TOKEN_H
