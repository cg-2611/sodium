#include "sodium/nac/token/token_cursor.h"

#include <optional>

#include "sodium/nac/token/token.h"
#include "sodium/nac/token/token_buffer.h"

namespace sodium {

TokenCursor::TokenCursor(const TokenBuffer &token_buffer)
        : token_buffer_(std::make_unique<TokenBuffer>(token_buffer)),
          index_(-1) {}

std::optional<Token> TokenCursor::next() {
    return token_buffer_->get(++index_);
}

std::optional<Token> TokenCursor::peek_ahead(size_t n) const {
    return token_buffer_->get(index_ + n);
}

size_t TokenCursor::index() const {
    return index_;
}

} // namespace sodium
