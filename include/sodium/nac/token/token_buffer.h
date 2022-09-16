#ifndef SODIUM_NAC_TOKEN_TOKEN_STREAM_H
#define SODIUM_NAC_TOKEN_TOKEN_STREAM_H

#include <optional>
#include <vector>

namespace sodium {

class Token;

/// A wrapper for an std::vector<Token> that is used to store and access tokens.
class TokenBuffer {
public:
    /// Default constructor for TokenBuffer.
    TokenBuffer() = default;

    /// Constructor for TokenBuffer.
    /// \param tokens An std::vector<Token> to construct the TokenBuffer with.
    TokenBuffer(std::vector<Token> tokens);

    /// Constructor for TokenBuffer.
    /// \param token A token that is to be pushed into the TokenBuffer on construction.
    TokenBuffer(const Token &token);

    /// Constructor for TokenBuffer.
    /// \param token A token that is to be pushed into the TokenBuffer on construction.
    TokenBuffer(Token &&token);

    /// Append a token to the end of \c this token buffer.
    /// \param token The token to be pushed.
    void push(Token token);

    /// \return An to std::optional<Token> with a value of the token at the specified index if the access into the
    /// vector was successful, or \c nullopt otherwise
    [[nodiscard]] std::optional<Token> get(size_t index) const;

    /// \return The number of tokens in the buffer.
    [[nodiscard]] size_t size() const;

private:
    std::vector<Token> tokens_;
};

} // namespace sodium

#endif // SODIUM_NAC_TOKEN_TOKEN_STREAM_H
