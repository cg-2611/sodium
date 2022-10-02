#include "sodium/nac/parser/parser.h"

#include <memory>
#include <string_view>
#include <utility>
#include <vector>

#include "sodium/nac/ast/ast.h"
#include "sodium/nac/ast/identifier.h"
#include "sodium/nac/ast/source_file.h"
#include "sodium/nac/ast/type.h"
#include "sodium/nac/basic/source_location.h"
#include "sodium/nac/basic/source_range.h"
#include "sodium/nac/diagnostics/diagnostic_engine.h"
#include "sodium/nac/parser/parser_diagnostics.h"
#include "sodium/nac/token/token.h"
#include "sodium/nac/token/token_buffer.h"
#include "sodium/nac/token/token_cursor.h"

namespace sodium {

Parser::Parser(const TokenBuffer &token_buffer, DiagnosticEngine &diagnostics)
        : diagnostics_(diagnostics),
          token_cursor_(token_buffer),
          token_(Token::dummy()) {
    advance();
}

AST Parser::parse() {
    // a source file is the root of the AST
    return AST(parse_source_file());
}

std::unique_ptr<SourceFile> Parser::parse_source_file() {
    auto start = token_.range().start();
    auto decls = std::vector<std::unique_ptr<Decl>>();

    while (!match(TokenKind::EOF_TOKEN)) {
        auto decl = parse_decl();
        if (!decl) {
            synchronize_decl();
            continue;
        }

        decls.push_back(std::move(decl));
    }

    return std::make_unique<SourceFile>(std::move(decls), start.to(token_.range().end()));
}

std::unique_ptr<Identifier> Parser::parse_identifier() {
    auto identifier_value = token_.value();
    auto identifier_range = token_.range();

    if (!expect(TokenKind::IDENTIFIER, ParserErrorKind::EXPECTED_IDENTIFIER)) {
        return nullptr;
    }

    return std::make_unique<Identifier>(std::move(identifier_value), identifier_range);
}

std::unique_ptr<Type> Parser::parse_type() {
    auto type_value = token_.value();
    auto type_range = token_.range();

    if (!expect(TokenKind::TYPE, ParserErrorKind::EXPECTED_TYPE)) {
        return nullptr;
    }

    return std::make_unique<Type>(std::move(type_value), type_range);
}

void Parser::advance() {
    token_ = token_cursor_.next().value_or(Token::dummy());
}

bool Parser::match(TokenKind expected) const {
    return token_.kind() == expected;
}

bool Parser::expect(TokenKind expected, ParserErrorKind kind) {
    if (!match(expected)) {
        error_expected(kind);
        return false;
    }

    advance();
    return true;
}

void Parser::error_expected(ParserErrorKind kind) const {
    auto parser_error = std::make_unique<ParserError>(kind, token_);
    diagnostics_.diagnose(std::move(parser_error));
}

void Parser::synchronize_decl() {
    while (!match(TokenKind::EOF_TOKEN)) {
        if (match(TokenKind::KEYWORD_FUNC)) {
            break;
        }

        advance();
    }
}

void Parser::synchronize_stmt() {
    while (!match(TokenKind::EOF_TOKEN)) {
        if (match(TokenKind::LEFT_BRACE) || match(TokenKind::RIGHT_BRACE)) {
            break;
        }

        if (match(TokenKind::SEMICOLON)) {
            advance();
            break;
        }

        advance();
    }
}

} // namespace sodium
