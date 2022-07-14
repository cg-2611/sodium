#ifndef SODIUM_NAC_LEXER_LEXER_H
#define SODIUM_NAC_LEXER_LEXER_H

#include "sodium/nac/io/file.h"
#include "sodium/nac/lexer/token.h"

namespace sodium::nac {

class Lexer {
public:
    Lexer(File &file);
    ~Lexer() {};

    std::shared_ptr<Token> tokenize();
    std::shared_ptr<Token> getNextToken();

    void advance(size_t offset);
    void skipWhitespace();

    std::string readIdentifier();
    std::string readNumericLiteral();

private:
    File file_;
    size_t index_;
    char character_;

    bool validIdentifierFirstCharacter(char c);
    bool validIdentifierCharacter(char c);

    bool isKeyword(const std::string &identifier);
    bool isType(const std::string &identifier);
};

} // namespace sodium::nac

#endif // SODIUM_NAC_LEXER_LEXER_H
