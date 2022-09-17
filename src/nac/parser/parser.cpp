#include "sodium/nac/parser/parser.h"

#include <initializer_list>
#include <memory>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include "sodium/nac/ast/ast.h"
#include "sodium/nac/ast/identifier.h"
#include "sodium/nac/ast/source_file.h"
#include "sodium/nac/ast/type.h"
#include "sodium/nac/basic/source_location.h"
#include "sodium/nac/basic/source_range.h"
#include "sodium/nac/errors/error.h"
#include "sodium/nac/errors/error_manager.h"
#include "sodium/nac/errors/parser_error.h"
#include "sodium/nac/token/token.h"
#include "sodium/nac/token/token_buffer.h"
#include "sodium/nac/token/token_cursor.h"
#include "sodium/nac/token/token_kind.h"

namespace sodium {

Parser::Parser(const TokenBuffer &token_buffer) : token_cursor_(token_buffer), token_(Token::dummy()) {
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
            synchronize({TokenKind::KEYWORD_FUNC});
            continue;
        }

        decls.push_back(std::move(decl));
    }

    return std::make_unique<SourceFile>(std::move(decls), start.to(token_.range().end()));
}

std::unique_ptr<Identifier> Parser::parse_identifier() {
    auto identifier_value = token_.value();
    auto identifier_range = token_.range();

    if (!expect(TokenKind::IDENTIFIER, "identifier")) {
        return nullptr;
    }

    return std::make_unique<Identifier>(std::move(identifier_value), identifier_range);
}

std::unique_ptr<Type> Parser::parse_type() {
    auto type_value = token_.value();
    auto type_range = token_.range();

    if (!expect(TokenKind::TYPE, "type")) {
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

bool Parser::expect(TokenKind expected, std::string_view message) {
    if (!match(expected)) {
        error_expected(message);
        return false;
    }

    advance();
    return true;
}

void Parser::error_expected(std::string_view message) const {
    std::string error_message("expected ");
    error_message += message;
    ErrorManager::add_error<ParserError>(ErrorKind::SYNTAX_ERROR, token_, error_message);
}

void Parser::synchronize(std::initializer_list<TokenKind> synchronizing_tokens) {
    while (!match(TokenKind::EOF_TOKEN)) {
        for (auto &&synchronizing_token : synchronizing_tokens) {
            if (match(synchronizing_token)) {
                return;
            }
        }

        advance();
    }
}

} // namespace sodium
