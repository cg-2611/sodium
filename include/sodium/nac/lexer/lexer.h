#ifndef SODIUM_NAC_LEXER_LEXER_H
#define SODIUM_NAC_LEXER_LEXER_H

#include <string_view>
#include <unordered_map>

#include "sodium/nac/lexer/token.h"

namespace sodium {

/**
 * @brief An std::unordered_map of the reserved words currently used by Sodium. The key is a string_view of the reserved
 *        word and the value is the corresponding TokenKind.
 *
 */
const std::unordered_map<std::string_view, TokenKind> RESERVED_WORDS{
  // keywords
    {  "func",   TokenKind::KEYWORD_FUNC},
    {"return", TokenKind::KEYWORD_RETURN},

 // types
    {   "int",           TokenKind::TYPE}
};

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
     * @brief Get the next token in the string. This will not return any tokens of kind TokenKind::ERROR_TOKEN.
     *
     * @return Token that is the next token in the string.
     */
    Token getNextToken();

private:
    const char *start_;   // the start of the current token being lexed
    const char *current_; // the current character being lexed
    const char *end_;     // the end of the string
    int line_;            // the current line of the string
    int column_;          // the current column of the current line

    // returns the next token in the string
    Token nextToken();

    // constructs and returns a token of the specified kind
    Token makeToken(TokenKind kind);

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
