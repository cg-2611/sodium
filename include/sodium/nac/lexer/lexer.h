#ifndef SODIUM_NAC_LEXER_LEXER_H
#define SODIUM_NAC_LEXER_LEXER_H

#include "sodium/nac/io/file.h"
#include "sodium/nac/lexer/token.h"

namespace sodium::nac {

/**
 * This class is used to extract the recognised sodium tokens from a string.
*/
class Lexer {
public:
    /**
     * Constructor for Lexer.
     * @param fileContents the string to be tokenized by the lexer.
    */
    Lexer(const std::string &fileContents);

    /**
     * Destructor for Lexer.
    */
    ~Lexer() {};

    /**
     * Extracts the tokens from the string stored when the lexer is constructed.
     * @return An std::shared_ptr<Token> pointer to the first token in the string.
    */
    std::shared_ptr<Token> tokenize();

    /**
     * @return An std::shared_ptr<Token> pointer to the next token in the string from
     * the current position of the lexer.
    */
    std::shared_ptr<Token> getNextToken();

    /**
     * Change the position of the lexer in the string. The lexer only moves forward in the string.
     * @param offset the number of characters to move the lexer forward in the string.
    */
    void advance(size_t offset);

    /**
     * Move the lexer over any consecutive whitespace characters.
    */
    void skipWhitespace();

    /**
     * Reads and constructs an identifier value from the current position of the lexer in the string.
     * This method does not modify the position of the lexer in the string, i.e. after the method returns,
     * the lexer will still be at the character at the start of the identifier.
     * @return The identifier read by the lexer as a string.
    */
    std::string readIdentifier();

    /**
     * Reads and constructs a numeric literal value (as a string) from the current position of the lexer in the string.
     * This method does not modify the position of the lexer in the string, i.e. after the method returns,
     * the lexer will still be at the character at the start of the numeric literal.
     * @return The numeric literal read by the lexer as a string.
    */
    std::string readNumericLiteral();

private:
    std::string fileContents_;
    size_t index_;
    char character_;

    inline bool validIdentifierFirstCharacter(char c);
    inline bool validIdentifierCharacter(char c);

    bool isKeyword(const std::string &identifier);
    bool isType(const std::string &identifier);
};

} // namespace sodium::nac

#endif // SODIUM_NAC_LEXER_LEXER_H
