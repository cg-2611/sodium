#include "sodium/nac/token/token_buffer.h"

#include <optional>
#include <utility>
#include <vector>

#include "sodium/nac/token/token.h"

namespace sodium {

TokenBuffer::TokenBuffer(std::vector<Token> tokens) : tokens_(std::move(tokens)) {}

TokenBuffer::TokenBuffer(const Token &token) {
    push(token);
}

void TokenBuffer::push(const Token &token) {
    tokens_.push_back(token);
}

std::optional<Token> TokenBuffer::get(size_t index) const {
    if (index >= size()) {
        return {};
    }

    return tokens_[index];
}

size_t TokenBuffer::size() const {
    return tokens_.size();
}

} // namespace sodium
