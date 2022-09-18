#include "sodium/nac/lexer/lexer.h"

#include <string_view>

#include "sodium/nac/basic/file.h"
#include "sodium/nac/basic/source_location.h"
#include "sodium/nac/errors/error.h"
#include "sodium/nac/errors/error_manager.h"
#include "sodium/nac/errors/lexer_error.h"
#include "sodium/nac/token/token.h"
#include "sodium/nac/token/token_buffer.h"

namespace sodium {

Lexer::Lexer(const File &file) : Lexer(file.contents().data()) {}

Lexer::Lexer(std::string_view src) : current_(src.begin()), end_(src.cend()), line_(1), column_(1), length_(0) {}

TokenBuffer Lexer::tokenize() {
    // return an eof token if we have an empty string
    if (at_end(current_)) {
        return TokenBuffer(make_token(TokenKind::EOF_TOKEN, {line_, column_, current_}));
    }

    auto token_buffer = TokenBuffer();

    while (!at_end(current_)) {
        auto token = next_token();

        // skip error tokens until we have a valid token
        while (token.kind() == TokenKind::ERROR_TOKEN) {
            token = next_token();
        }

        token_buffer.push(token);
    }

    return token_buffer;
}

Token Lexer::next_token() {
    skip_whitespace();

    length_ = 0; // reset the length of the current token

    const auto *first_char = current_;
    auto token_start = SourceLocation(line_, column_, first_char);

    advance();

    if (at_end(first_char)) {
        return make_token(TokenKind::EOF_TOKEN, token_start);
    }

    // if we have an identifier, handle it
    if (is_identifier_start(*first_char)) {
        consume_identifier();

        auto identifier = std::string_view(token_start.position(), length_);
        if (is_reserved(identifier)) {
            return make_token(RESERVED_WORDS.at(identifier), token_start);
        }

        return make_token(TokenKind::IDENTIFIER, token_start);
    }

    // if we have an integer literal, handle it
    if (is_digit(*first_char)) {
        consume_integer_literal();
        return make_token(TokenKind::INTEGER_LITERAL, token_start);
    }

    switch (*first_char) {
        case '{': return make_token(TokenKind::LEFT_BRACE, token_start);
        case '}': return make_token(TokenKind::RIGHT_BRACE, token_start);
        case '(': return make_token(TokenKind::LEFT_PAREN, token_start);
        case ')': return make_token(TokenKind::RIGHT_PAREN, token_start);
        case ';': return make_token(TokenKind::SEMICOLON, token_start);
        case '-':
            // if the current token starts with a '-' and the next character is a '>'
            if (*current_ == '>') {
                advance();
                return make_token(TokenKind::ARROW, token_start);
            }
            [[fallthrough]]; // temporary until '-' is a valid token
        default:
            auto error_token = make_token(TokenKind::ERROR_TOKEN, token_start);
            ErrorManager::add_error<LexerError>(ErrorKind::UNRECOGNISED_TOKEN, error_token);
            return error_token;
    }
}

Token Lexer::make_token(TokenKind kind, SourceLocation start) const {
    return Token(kind, start.to({line_, column_, current_}), length_);
}

void Lexer::consume_identifier() {
    while (is_identifier_start(*current_) || is_digit(*current_)) {
        advance();
    }
}

void Lexer::consume_integer_literal() {
    while (is_digit(*current_)) {
        advance();
    }
}

void Lexer::advance() {
    if (!at_end(current_)) {
        ++current_;
        ++column_;
        ++length_;
    }
}

void Lexer::skip_whitespace() {
    while (is_space(*current_)) {
        if (*current_ == '\n') {
            handle_newline();
        }

        advance();
    }
}

void Lexer::handle_newline() {
    ++line_;
    column_ = 0; // reset the column number
}

bool Lexer::at_end(std::string_view::const_iterator iterator) const {
    return iterator >= end_;
}

} // namespace sodium
