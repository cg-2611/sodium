#include "sodium/nac/ast/expr.h"

#include "sodium/nac/ast/ast_node.h"
#include "sodium/nac/ast/ast_visitor.h"

namespace sodium {

Expr::Expr(ExprKind kind) : ASTNode(ASTNodeKind::EXPR), kind_(kind) {}

void Expr::accept(ASTVisitor *visitor) const {
    visitor->visit(this);
}

ExprKind Expr::exprKind() const noexcept {
    return kind_;
}

LiteralExpr::LiteralExpr(LiteralKind kind) : Expr(ExprKind::LITERAL), kind_(kind) {}

void LiteralExpr::accept(ASTVisitor *visitor) const {
    visitor->visit(this);
}

LiteralKind LiteralExpr::literalKind() const noexcept {
    return kind_;
}

NumericLiteralExpr::NumericLiteralExpr(int value) : LiteralExpr(LiteralKind::NUMERIC_LITERAL), value_(value) {}

void NumericLiteralExpr::accept(ASTVisitor *visitor) const {
    visitor->visit(this);
}

int NumericLiteralExpr::value() const noexcept {
    return value_;
}

} // namespace sodium
