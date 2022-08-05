#ifndef SODIUM_NAC_LEXER_LEXER_H
#define SODIUM_NAC_LEXER_LEXER_H

#include <memory>
#include <unordered_set>
#include <string_view>

#include "sodium/nac/lexer/token.h"

namespace sodium {

/// An std::unordered_set of the keywords currently used in Sodium.
const std::unordered_set<std::string_view> KEYWORDS{"func", "return"};
/// An std::unordered_set of the types currently used in Sodium.
const std::unordered_set<std::string_view> TYPES{"int"};

/**
 * Used to extract, from a string, any tokens used in the Sodium programming language.
 */
class Lexer {
public:
    /**
     * Constructor for Lexer.
     * @param src The string to be tokenized.
     */
    Lexer(std::string_view src);

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
    const char *start_;   // the start of the current token being lexed
    const char *current_; // the current character being lexed
    const char *end_;     // the end of the string
    size_t line_;
    size_t column_;

    // returns a unique pointer to the next token in the string
    [[nodiscard]] std::unique_ptr<Token> getNextToken();

    // constructs and returns a token of the specified kind
    [[nodiscard]] std::unique_ptr<Token> makeToken(TokenKind kind);

    // advances the lexer to the character after the end of an identifier
    size_t readIdentifier();

    // advances the lexer to the character after the end of a numeric literal
    size_t readNumericLiteral();

    void advance() noexcept;
    void skipWhitespace() noexcept;
    inline bool atEndOfString() const noexcept; // returns true if the end of the string is reached
    inline bool atEndOfLine() const noexcept; // returns true if the end of the current line is reached
};

} // namespace sodium

#endif // SODIUM_NAC_LEXER_LEXER_H
