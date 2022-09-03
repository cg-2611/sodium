#ifndef SODIUM_NAC_LEXER_LEXER_H
#define SODIUM_NAC_LEXER_LEXER_H

#include <memory>
#include <string_view>
#include <unordered_set>

#include "sodium/nac/lexer/token.h"

namespace sodium {

/// @brief std::unordered_set of the keywords currently used in Sodium.
const std::unordered_set<std::string_view> KEYWORDS{"func", "return"};

/// @brief An std::unordered_set of the types currently used in Sodium.
const std::unordered_set<std::string_view> TYPES{"int"};

/**
 * @brief Used to extract, from a string, any tokens used in the Sodium programming language.
 *
 */
class Lexer {
public:
    /**
     * @brief Construct a new Lexer object.
     *
     * @param src The std::string_view to be tokenized.
     */
    Lexer(std::string_view src);

    /**
     * @brief Extracts the Sodium programming language tokens from the string.
     *
     * @return std::unique_ptr<Token> to the first token in the string.
     */
    [[nodiscard]] std::unique_ptr<Token> tokenize();

private:
    const char *start_;   // the start of the current token being lexed
    const char *current_; // the current character being lexed
    const char *end_;     // the end of the string
    size_t line_;         // the current line of the string
    size_t column_;       // the current column of the current line

    // returns a unique pointer to the next token in the string
    [[nodiscard]] std::unique_ptr<Token> getNextToken();

    // constructs and returns a token of the specified kind
    [[nodiscard]] std::unique_ptr<Token> makeToken(TokenKind kind);

    // advances the lexer to the character after the end of an identifier
    size_t readIdentifier();

    // advances the lexer to the character after the end of a numeric literal
    size_t readNumericLiteral();

    void advance() noexcept;                    // advance current_ to the next character in the string
    void skipWhitespace() noexcept;             // skip whitespace characters that are not '\n'
    inline bool atEndOfString() const noexcept; // returns true if the end of the string is reached
    inline bool atEndOfLine() const noexcept;   // returns true if the end of the current line is reached
};

} // namespace sodium

#endif // SODIUM_NAC_LEXER_LEXER_H
