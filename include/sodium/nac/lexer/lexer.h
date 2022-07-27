#ifndef SODIUM_NAC_LEXER_LEXER_H
#define SODIUM_NAC_LEXER_LEXER_H

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
    [[nodiscard]] std::unique_ptr<Token> tokenize();

private:
    std::string_view string_;
    size_t index_;

    // returns the next token in the string from the current position of the lexer
    // note: index_ is not updated
    std::unique_ptr<Token> getNextToken();

    // increase index_ by offset characters
    // index_ will not exceed the length of the string
    constexpr void advance(size_t offset);

    // moves the lexer over all consecutive whitespace characters
    constexpr void skipWhitespace();

    constexpr size_t getIdentifierLength();     // returns the length of an identifier from index_
    constexpr size_t getNumericLiteralLength(); // returns the length of a numeric literal from index_
};

} // namespace nac

#endif // SODIUM_NAC_LEXER_LEXER_H
