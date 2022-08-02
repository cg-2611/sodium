#ifndef SODIUM_NAC_LEXER_LEXER_H
#define SODIUM_NAC_LEXER_LEXER_H

#include <memory>
#include <unordered_set>
#include <string_view>

#include "sodium/nac/lexer/token.h"

namespace nac {

/// An std::unordered_set of the keywords (as strings) currently used in Sodium.
const std::unordered_set<std::string_view> KEYWORDS{"func", "return"};
/// An std::unordered_set of the types (as strings) currently used in Sodium.
const std::unordered_set<std::string_view> TYPES{"int"};

/**
 * Used to extract, from a string, any tokens used in the Sodium programming language.
 */
class Lexer {
public:
    /**
     * Constructor for Lexer. Initializes private members.
     * @param string The string to be tokenized.
     */
    Lexer(std::string_view src);

    /**
     * Destructor for Lexer.
     */
    ~Lexer() = default;

    /**
     * Extracts the Sodium programming language tokens from the string.
     * @return An std::unique_ptr<Token> pointer to the first token in the string.
     * @throws An nac::LexerException when an unrecognised token is encountered.
     */
    [[nodiscard]] std::unique_ptr<Token> tokenize();

private:
    const char *start_;
    const char *current_;
    const char *end_;
    size_t line_;
    size_t column_;

    std::unique_ptr<Token> getNextToken();
    [[nodiscard]] std::unique_ptr<Token> makeToken(TokenKind kind);

    size_t readIdentifier();
    size_t readNumericLiteral();

    void advance() noexcept;
    void skipWhitespace() noexcept;
    inline bool atEnd() const noexcept;
};

} // namespace nac

#endif // SODIUM_NAC_LEXER_LEXER_H
