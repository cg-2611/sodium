#ifndef SODIUM_NAC_LEXER_TOKEN_H
#define SODIUM_NAC_LEXER_TOKEN_H

#include <set>
#include <string>

namespace sodium::nac {

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

class Token {
public:
    static const std::set<std::string> KEYWORDS;
    static const std::set<std::string> TYPES;

    Token(TokenKind tokenKind, const std::string tokenValue);
    ~Token() {};

    TokenKind getKind() const;
    const std::string getKindAsString() const;
    const std::string getValue() const;
    const std::shared_ptr<Token> getNextToken() const;

    void setNextToken(std::shared_ptr<Token> nextToken);

private:
    TokenKind kind_;
    std::string value_;
    std::shared_ptr<Token> next_;
};

} // namespace sodium::nac

#endif // SODIUM_NAC_LEXER_TOKEN_H
