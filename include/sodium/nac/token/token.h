#ifndef SODIUM_NAC_TOKEN_TOKEN_H
#define SODIUM_NAC_TOKEN_TOKEN_H

#include <string>

#include "sodium/nac/basic/source_range.h"

namespace sodium {

/// An enum of the different kinds of tokens currently used in Sodium.
enum class TokenKind {
    // reserved tokens
    KEYWORD_FUNC,
    KEYWORD_RETURN,
    TYPE,

    // literal tokens
    IDENTIFIER,
    INTEGER_LITERAL,

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

/// A Sodium programming language token.
class Token {
public:
    /// Constructor for Token.
    /// \param kind The kind of token being constructed.
    /// \param range The SourceRange of the token.
    /// \param length The length of the token.
    Token(TokenKind kind, SourceRange range, int length);

    /// \return The kind of \c this token.
    [[nodiscard]] TokenKind kind() const;

    /// \return The range of \c this token.
    [[nodiscard]] SourceRange range() const;

    /// \return The length of \c this token.
    [[nodiscard]] int length() const;

    /// \return The value of \c this token.
    [[nodiscard]] std::string value() const;

    static Token dummy();

private:
    TokenKind kind_;
    SourceRange range_;
    int length_;
};

} // namespace sodium

#endif // SODIUM_NAC_TOKEN_TOKEN_H
