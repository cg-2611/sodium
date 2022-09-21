#include "sodium/nac/ast/stmt.h"

#include <memory>
#include <utility>
#include <vector>

#include "sodium/nac/ast/ast_node.h"
#include "sodium/nac/ast/ast_visitor.h"
#include "sodium/nac/ast/expr.h"
#include "sodium/nac/basic/source_range.h"

namespace sodium {

Stmt::Stmt(StmtKind kind, SourceRange range) : ASTNode(ASTNodeKind::STMT, range), kind_(kind) {}

StmtKind Stmt::stmt_kind() const {
    return kind_;
}

Block::Block(std::vector<std::unique_ptr<Stmt>> stmts, SourceRange range)
        : Stmt(StmtKind::BLOCK, range),
          stmts_(std::move(stmts)) {}

void Block::accept(ASTVisitor &visitor) const {
    visitor.visit(*this);
}

const std::vector<std::unique_ptr<Stmt>> &Block::stmts() const {
    return stmts_;
}

ReturnStmt::ReturnStmt(std::unique_ptr<Expr> expr, SourceRange range)
        : Stmt(StmtKind::RETURN, range),
          expr_(std::move(expr)) {}

void ReturnStmt::accept(ASTVisitor &visitor) const {
    visitor.visit(*this);
}

Expr *ReturnStmt::expr() const {
    return expr_.get();
}

} // namespace sodium
