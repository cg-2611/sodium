#ifndef NAC_LEXER_TOKEN_H
#define NAC_LEXER_TOKEN_H

#include <set>
#include <string>

namespace nac {

/// An std::set of the keywords (as strings) currently used in Sodium.
const std::set<std::string> KEYWORDS{
    "func", "return"
};
/// An std::set of the types (as strings) currently used in Sodium.
const std::set<std::string> TYPES{
    "int"
};

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
    Token(TokenKind kind, const std::string &string);

    /**
     * Destructor for Token.
    */
    ~Token() {}

    /**
     * @return The kind of this token.
    */
    TokenKind getKind() const;

    /**
     * @return The kind of this token as a string. The string will match the
     * corresponding nac::TokenKind enum identifier.
    */
    const std::string getKindAsString() const;

    /**
     * @return The value of this token as a string.
    */
    const std::string & getValue() const;

private:
    TokenKind kind_;
    std::string value_;
};

} // namespace nac

#endif // NAC_LEXER_TOKEN_H
