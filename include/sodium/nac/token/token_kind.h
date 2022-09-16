#ifndef SODIUM_NAC_TOKEN_TOKEN_KIND_H
#define SODIUM_NAC_TOKEN_TOKEN_KIND_H

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

} // namespace sodium

#endif // SODIUM_NAC_TOKEN_TOKEN_KIND_H
