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

static const std::unordered_set<TokenKind> STMT_SYNCHRONIZING_TOKENS = {TokenKind::RIGHT_BRACE, TokenKind::SEMICOLON};

std::unique_ptr<Stmt> Parser::parseStmt() {
    switch (token_->kind()) {
        case TokenKind::LEFT_BRACE: return parseBlock();
        case TokenKind::KEYWORD:
            if (token_->value() == "return") {
                std::unique_ptr<ReturnStmt> returnStmt(parseReturnStmt());
                if (returnStmt == nullptr) {
                    // expected return statement
                    ErrorManager::addError<ParserError>(ErrorKind::SYNTAX_ERROR, token_, "expected return statement");
                    synchronize(STMT_SYNCHRONIZING_TOKENS);
                    return nullptr;
                }
                return returnStmt;
            }
            [[fallthrough]];
        default:
            // expected statement
            ErrorManager::addError<ParserError>(ErrorKind::SYNTAX_ERROR, token_, "expected statement");
            synchronize(STMT_SYNCHRONIZING_TOKENS);
            return nullptr;
    }
}

std::unique_ptr<Block> Parser::parseBlock() {
    if (token_->kind() != TokenKind::LEFT_BRACE) {
        // expected {
        ErrorManager::addError<ParserError>(ErrorKind::SYNTAX_ERROR, token_, "expected { to start block");
    }

    std::vector<std::unique_ptr<Stmt>> stmts{};

    // parse statements until we reach the end of the block
    while (nextToken()->kind() != TokenKind::RIGHT_BRACE && nextToken()->kind() != TokenKind::EOF_TOKEN) {
        advance(); // advance to expected statement
        stmts.push_back(parseStmt());
    }

    advance(); // advance to the } token

    if (token_->kind() != TokenKind::RIGHT_BRACE) {
        // expected }, brace pair never closed
        ErrorManager::addError<ParserError>(ErrorKind::SYNTAX_ERROR, token_,
                                            "expected } to end block, pair never closed");
        return nullptr;
    }

    return std::make_unique<Block>(std::move(stmts));
}

std::unique_ptr<ReturnStmt> Parser::parseReturnStmt() {
    advance(); // advance to expected return expression

    // parse the expression being returned
    std::unique_ptr<Expr> expr(parseExpr());

    if (expr == nullptr) {
        // expected expression to return
        ErrorManager::addError<ParserError>(ErrorKind::SYNTAX_ERROR, token_, "expected expression to return");
        return nullptr;
    }

    advance(); // advance to expected semicolon

    if (token_->kind() != TokenKind::SEMICOLON) {
        ErrorManager::addError<ParserError>(ErrorKind::SYNTAX_ERROR, token_, "expected ; at end of return statement");
        return nullptr;
    }

    return std::make_unique<ReturnStmt>(std::move(expr));
}

} // namespace sodium
