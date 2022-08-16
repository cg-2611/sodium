#include "sodium/nac/parser/parser.h"

#include <memory>
#include <utility>
#include <vector>

#include "sodium/nac/ast/expr.h"
#include "sodium/nac/ast/stmt.h"
#include "sodium/nac/lexer/token.h"

namespace sodium {

// Statement = ReturnStatement | Block ;
std::unique_ptr<Stmt> Parser::parseStmt() {
    std::unique_ptr<Stmt> stmt(nullptr);

    if (token_->kind() == TokenKind::KEYWORD && token_->value() == "return") {
        stmt = parseReturnStmt();
    } else if (token_->kind() == TokenKind::LEFT_BRACE) {
        stmt = parseBlock();
    }

    // error, expected a return statement, but did not receive one

    return stmt;
}

// Block = { Statement* } ;
std::unique_ptr<Block> Parser::parseBlock() {
    if (token_->kind() != TokenKind::LEFT_BRACE) {
        // error, expected a block, but did not receive one
    }

    skipExcessEOLTokens(); // skip excess EOL tokens after { token

    std::vector<std::unique_ptr<Stmt>> stmts{};

    // parse statements until we reach the end of the block
    while (nextToken()->kind() != TokenKind::RIGHT_BRACE) {
        advance(); // advance to expected statement
        stmts.push_back(parseStmt());

        skipExcessEOLTokens(); // skip excess EOL tokens after statement
    }

    advance(); // advance to the } token

    // TODO: error if brace pair never closed

    return std::make_unique<Block>(std::move(stmts));
}

// ReturnStatement = return Expression StatementTerminator ;
std::unique_ptr<ReturnStmt> Parser::parseReturnStmt() {
    advance(); // advance to expected return expression

    // parse the expression being returned
    std::unique_ptr<Expr> expr(parseExpr());

    if (!isStmtTerminated()) {
        // error, expected a statement terminator, but not present
    }

    return std::make_unique<ReturnStmt>(std::move(expr));
}

// StatementTerminator = EOL | } ;
bool Parser::isStmtTerminated() {
    return nextToken()->kind() == TokenKind::EOL_TOKEN || nextToken()->kind() == TokenKind::RIGHT_BRACE;
}

} // namespace sodium
