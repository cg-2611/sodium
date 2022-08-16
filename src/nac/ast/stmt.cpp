#include "sodium/nac/ast/stmt.h"

#include <memory>
#include <utility>
#include <vector>

#include "sodium/nac/ast/ast_node.h"
#include "sodium/nac/ast/ast_visitor.h"
#include "sodium/nac/ast/expr.h"

namespace sodium {

Stmt::Stmt(StmtKind kind) : ASTNode(ASTNodeKind::STMT), kind_(kind) {}

void Stmt::accept(ASTVisitor *visitor) const {
    visitor->visit(this);
}

StmtKind Stmt::stmtKind() const noexcept {
    return kind_;
}

Block::Block(std::vector<std::unique_ptr<Stmt>> stmts) : Stmt(StmtKind::BLOCK), stmts_(std::move(stmts)) {}

void Block::accept(ASTVisitor *visitor) const {
    // call the accept method of all the statements in the block
    for (auto &&stmt : stmts_) {
        stmt->accept(visitor);
    }

    visitor->visit(this);
}

const std::vector<std::unique_ptr<Stmt>> &Block::stmts() const noexcept {
    return stmts_;
}

ReturnStmt::ReturnStmt(std::unique_ptr<Expr> expr) : Stmt(StmtKind::RETURN), expr_(std::move(expr)) {}

void ReturnStmt::accept(ASTVisitor *visitor) const {
    expr_->accept(visitor);
    visitor->visit(this);
}

Expr *ReturnStmt::expr() const noexcept {
    return expr_.get();
}

} // namespace sodium
