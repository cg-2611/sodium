#include "sodium/nac/exceptions/lexer_exception.h"

namespace nac {

LexerException::LexerException(const Error error, const char token) : Exception(error) {
    message_ += " \'\'";

    // insert a single character, token, in the penultimate position in message_
    message_.insert(message_.size() - 1, 1, token);
}

const char* LexerException::what() const noexcept {
    return message_.c_str();
}

} // namespace nac
