#ifndef NAC_LEXER_LEXER_H
#define NAC_LEXER_LEXER_H

#include <memory>

#include "sodium/nac/io/file.h"
#include "sodium/nac/lexer/token.h"

namespace nac {

/**
 * Used to extract, from a string, any tokens used in the Sodium programming language.
 */
class Lexer {
  public:
    /**
     * Constructor for Lexer. Initializes private members.
     * @param string The string to be tokenized.
     */
    Lexer(std::string_view string);

    /**
     * Destructor for Lexer.
     */
    ~Lexer() = default;

    /**
     * Extracts the Sodium programming language tokens from the string.
     * @return An std::unique_ptr<Token> pointer to the first token in the string.
     */
    std::unique_ptr<Token> tokenize();

  private:
    std::string_view string_;
    size_t index_;

    // returns the next token in the string from the current position of the lexer
    // note: index_ is not updated
    std::unique_ptr<Token> getNextToken();

    // increase index_ by offset characters
    // index_ will not exceed the length of the string
    void advance(size_t offset);

    // moves the lexer over all consecutive whitespace characters
    void skipWhitespace();

    size_t getIdentifierLength();     // returns the length of an identifier from index_
    size_t getNumericLiteralLength(); // returns the length of a numeric literal from index_

    // returns true if c is a valid character to begin an identifier
    inline bool validIdentifierFirstCharacter(char c);

    // returns true if c is a valid character to be anywhere in the identifier after the first character
    inline bool validIdentifierCharacter(char c);

    inline bool isKeyword(const std::string& identifier); // returns true if identifier is a keyword
    inline bool isType(const std::string& identifier);    // returns true if identifier is a type
};

} // namespace nac

#endif // NAC_LEXER_LEXER_H
