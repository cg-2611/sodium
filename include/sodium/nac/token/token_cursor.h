#ifndef SODIUM_NAC_TOKEN_TOKEN_ITERATOR_H
#define SODIUM_NAC_TOKEN_TOKEN_ITERATOR_H

#include <memory>
#include <optional>

#include "sodium/nac/token/token_buffer.h"

namespace sodium {

class Token;

/// Used to iterate over a TokenBuffer.
class TokenCursor {
public:
    /// Constructor for TokenCursor.
    /// \param token_buffer The TokenBuffer to iterate. The TokenBuffer must outlive the TokenCursor.
    TokenCursor(const TokenBuffer &token_buffer);

    /// \return An to std::optional<Token> with a value of the next Token if the access into the TokenBuffer was
    /// successful, or \c nullopt otherwise.
    std::optional<Token> next();

    /// Get the token at an arbitrary position from the current index.
    /// \param n The number of tokens ahead of the curren index to access.
    /// \return An to std::optional<Token> with a value of the requested Token if the access into the TokenBuffer was
    /// successful, or \c nullopt otherwise.
    [[nodiscard]] std::optional<Token> peek_ahead(size_t n) const;

    /// \return The current index of \c this cursor.
    [[nodiscard]] size_t index() const;

private:
    std::unique_ptr<TokenBuffer> token_buffer_;
    size_t index_;
};

} // namespace sodium

#endif // SODIUM_NAC_TOKEN_TOKEN_ITERATOR_H
