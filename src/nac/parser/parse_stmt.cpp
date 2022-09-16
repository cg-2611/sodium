#include "sodium/nac/parser/parser.h"

#include <memory>
#include <utility>
#include <vector>

#include "sodium/nac/ast/expr.h"
#include "sodium/nac/ast/stmt.h"
#include "sodium/nac/basic/source_range.h"
#include "sodium/nac/errors/error_manager.h"
#include "sodium/nac/errors/parser_error.h"
#include "sodium/nac/token/token.h"
#include "sodium/nac/token/token_kind.h"

namespace sodium {

std::unique_ptr<Stmt> Parser::parse_stmt() {
    switch (token_.kind()) {
        case TokenKind::LEFT_BRACE: return parse_block();
        case TokenKind::KEYWORD_RETURN: return parse_return_stmt();
        default: error_expected("statement"); return nullptr;
    }
}

std::unique_ptr<Block> Parser::parse_block() {
    auto left_brace_location = token_.range().start();

    if (!expect(TokenKind::LEFT_BRACE, "{ to begin block")) {
        return nullptr;
    }

    auto stmts = std::vector<std::unique_ptr<Stmt>>();

    while (!match(TokenKind::RIGHT_BRACE) && !match(TokenKind::EOF_TOKEN)) {
        auto stmt = parse_stmt();
        if (!stmt) {
            synchronize({TokenKind::LEFT_BRACE, TokenKind::RIGHT_BRACE, TokenKind::SEMICOLON});
            continue;
        }

        stmts.push_back(std::move(stmt));
    }

    auto right_brace_location = token_.range().end();

    if (!expect(TokenKind::RIGHT_BRACE, "} to end block")) {
        return nullptr;
    }

    return std::make_unique<Block>(std::move(stmts), left_brace_location.to(right_brace_location));
}

std::unique_ptr<ReturnStmt> Parser::parse_return_stmt() {
    auto return_keyword_location = token_.range().start();

    if (!expect(TokenKind::KEYWORD_RETURN, "return keyword")) {
        return nullptr;
    }

    std::unique_ptr<Expr> return_expr(parse_expr());
    if (!return_expr) {
        return nullptr;
    }

    auto semicolon_location = token_.range().end();

    if (!expect(TokenKind::SEMICOLON, ";")) {
        return nullptr;
    }

    return std::make_unique<ReturnStmt>(std::move(return_expr), return_keyword_location.to(semicolon_location));
}

} // namespace sodium
