#ifndef SODIUM_NAC_LEXER_TOKEN_H
#define SODIUM_NAC_LEXER_TOKEN_H

#include <memory>
#include <set>
#include <string>

namespace nac {

/// An std::set of the keywords (as strings) currently used in Sodium.
const std::set<std::string> KEYWORDS{"func", "return"};
/// An std::set of the types (as strings) currently used in Sodium.
const std::set<std::string> TYPES{"int"};

/**
 * An enum of the different kinds of tokens currently used in Sodium.
 */
enum class TokenKind {
    TOKEN_EOF,
    TOKEN_KEYWORD,
    TOKEN_TYPE,
    TOKEN_IDENTIFIER,
    TOKEN_NUMERIC_LITERAL,
    TOKEN_COLON,
    TOKEN_LEFT_BRACE,
    TOKEN_LEFT_PAREN,
    TOKEN_RIGHT_BRACE,
    TOKEN_RIGHT_PAREN,
    TOKEN_SEMI_COLON
};

/**
 * A Sodium programming language token.
 */
class Token {
public:
    /**
     * Constructor for a token.
     * @param tokenKind the kind of token being constructed, must be one of nac::TokenKind.
     * @param tokenValue the value of the token as a string.
     */
    Token(TokenKind kind, std::string value);

    /// Explicitly delete copy constructor as unique_ptr is non-copyable.
    Token(const Token& other) = delete;

    /**
     * Default move constructor.
     * @param other The token of which to move the resources from into this token.
     */
    Token(Token&& other) noexcept = default;

    /**
     * Destructor for Token.
     */
    ~Token() = default;

    /// Explicitly delete copy assignment operator as unique_ptr is non-copyable.
    Token& operator=(const Token& other) = delete;

    /**
     * Default move assignment operator.
     * @param other The token which is being move assigned to this token.
     */
    Token& operator=(Token&& other) noexcept = default;

    /**
     * @return The kind of this token.
     */
    TokenKind kind() const noexcept;

    /**
     * @return The value of this token as a string.
     */
    const std::string& value() const noexcept;

    /**
     * @return A raw pointer to the token that comes after this token.
     */
    Token* next() const noexcept;

    /**
     * Modifier method for the next token.
     * @param next An std::unique_ptr<Token> which is moved into the next_ member of this token.
     */
    void next(std::unique_ptr<Token> next) noexcept;

private:
    TokenKind kind_;
    std::string value_;
    std::unique_ptr<Token> next_;
};

} // namespace nac

#endif // SODIUM_NAC_LEXER_TOKEN_H
