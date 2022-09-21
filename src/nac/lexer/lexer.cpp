#include "sodium/nac/lexer/lexer.h"

#include <memory>
#include <string_view>

#include "sodium/nac/basic/file.h"
#include "sodium/nac/basic/source_location.h"
#include "sodium/nac/diagnostics/diagnostic_engine.h"
#include "sodium/nac/lexer/lexer_diagnostics.h"
#include "sodium/nac/token/token.h"
#include "sodium/nac/token/token_buffer.h"

namespace sodium {

Lexer::Lexer(std::string_view src, DiagnosticEngine &diagnostics)
        : diagnostics_(diagnostics),
          token_start_(src.begin()),
          current_char_(src.begin()),
          string_end_(src.cend()),
          line_(1),
          column_(1),
          length_(0) {}

TokenBuffer Lexer::tokenize() {
    // return an eof token if we have an empty string
    if (at_end()) {
        return TokenBuffer(make_token(TokenKind::EOF_TOKEN, {line_, column_, current_char_}));
    }

    auto token_buffer = TokenBuffer();

    while (!at_end()) {
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

    token_start_ = current_char_;
    auto start_location = SourceLocation(line_, column_, token_start_);

    advance();

    if (at_end()) {
        return make_token(TokenKind::EOF_TOKEN, start_location);
    }

    // if we have an identifier, handle it
    if (is_identifier_start(*token_start_)) {
        consume_identifier();

        auto identifier = std::string_view(start_location.position(), length_);
        if (is_reserved(identifier)) {
            return make_token(RESERVED_WORDS.at(identifier), start_location);
        }

        return make_token(TokenKind::IDENTIFIER, start_location);
    }

    // if we have an integer literal, handle it
    if (is_digit(*token_start_)) {
        consume_integer_literal();
        return make_token(TokenKind::INTEGER_LITERAL, start_location);
    }

    switch (*token_start_) {
        case '{': return make_token(TokenKind::LEFT_BRACE, start_location);
        case '}': return make_token(TokenKind::RIGHT_BRACE, start_location);
        case '(': return make_token(TokenKind::LEFT_PAREN, start_location);
        case ')': return make_token(TokenKind::RIGHT_PAREN, start_location);
        case ';': return make_token(TokenKind::SEMICOLON, start_location);
        case '-':
            // if the current token starts with a '-' and the next character is a '>'
            if (*current_char_ == '>') {
                advance();
                return make_token(TokenKind::ARROW, start_location);
            }
            [[fallthrough]]; // temporary until '-' is a valid token
        default: return make_lexer_error(LexerErrorKind::UNRECOGNISED_TOKEN, start_location);
    }
}

Token Lexer::make_token(TokenKind kind, SourceLocation start) const {
    return Token(kind, start.to({line_, column_, current_char_}), length_);
}

Token Lexer::make_lexer_error(LexerErrorKind kind, SourceLocation location) const {
    auto error_token = make_token(TokenKind::ERROR_TOKEN, location);
    auto lexer_error = std::make_unique<LexerError>(kind, error_token);

    diagnostics_.diagnose(std::move(lexer_error));

    return error_token;
}

void Lexer::consume_identifier() {
    while (is_identifier_start(*current_char_) || is_digit(*current_char_)) {
        advance();
    }
}

void Lexer::consume_integer_literal() {
    while (is_digit(*current_char_)) {
        advance();
    }
}

void Lexer::advance() {
    if (!at_end()) {
        ++current_char_;
        ++column_;
        ++length_;
    }
}

void Lexer::skip_whitespace() {
    while (is_space(*current_char_)) {
        if (*current_char_ == '\n') {
            handle_newline();
        }

        advance();
    }
}

void Lexer::handle_newline() {
    ++line_;
    column_ = 0; // reset the column number
}

bool Lexer::at_end() const {
    return token_start_ >= string_end_;
}

} // namespace sodium
