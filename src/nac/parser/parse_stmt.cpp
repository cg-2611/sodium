#include "sodium/nac/parser/parser.h"

#include <memory>
#include <unordered_set>
#include <utility>
#include <vector>

#include "sodium/nac/ast/expr.h"
#include "sodium/nac/ast/stmt.h"
#include "sodium/nac/errors/error_manager.h"
#include "sodium/nac/errors/parser_error.h"
#include "sodium/nac/lexer/token.h"

namespace sodium {

std::unique_ptr<Stmt> Parser::parseStmt() {
    switch (token_->kind()) {
        case TokenKind::LEFT_BRACE: return parseBlock();
        case TokenKind::KEYWORD:
            if (token_->value() == "return") {
                return parseReturnStmt();
            }
            [[fallthrough]];
        default: errorExpected("statement"); return nullptr;
    }
}

std::unique_ptr<Block> Parser::parseBlock() {
    if (!expect(TokenKind::LEFT_BRACE, "{ to start block")) {
        return nullptr;
    }

    advance(); // advance to statements

    std::vector<std::unique_ptr<Stmt>> stmts{};

    // parse statements until we reach the end of the block
    while (token_->kind() != TokenKind::RIGHT_BRACE && token_->kind() != TokenKind::EOF_TOKEN) {
        std::unique_ptr<Stmt> stmt(parseStmt());
        if (!stmt) {
            synchronize(STMT_SYNCHRONIZING_TOKENS);
        }

        stmts.push_back(std::move(stmt));

        if (token_->kind() == TokenKind::SEMICOLON) {
            advance(); // advance to next statement
        }
    }

    if (!expect(TokenKind::RIGHT_BRACE, "} to end block, pair never closed")) {
        return nullptr;
    }

    advance(); // advance to token after block to continue parsing

    return std::make_unique<Block>(std::move(stmts));
}

std::unique_ptr<ReturnStmt> Parser::parseReturnStmt() {
    advance(); // advance to expected return expression

    // parse the expression being returned
    std::unique_ptr<Expr> expr(parseExpr());
    if (!expr) {
        errorExpected("expression to return");
        return nullptr;
    }

    advance(); // advance to expected ;

    if (!expect(TokenKind::SEMICOLON, "; after return statement")) {
        return nullptr;
    }

    return std::make_unique<ReturnStmt>(std::move(expr));
}

} // namespace sodium
