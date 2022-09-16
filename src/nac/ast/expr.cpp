#include "sodium/nac/ast/expr.h"

#include "sodium/nac/ast/ast_node.h"
#include "sodium/nac/ast/ast_visitor.h"
#include "sodium/nac/basic/source_range.h"

namespace sodium {

Expr::Expr(ExprKind kind, SourceRange range) : ASTNode(ASTNodeKind::EXPR, range), kind_(kind) {}

ExprKind Expr::expr_kind() const {
    return kind_;
}

IntegerLiteralExpr::IntegerLiteralExpr(int value, SourceRange range)
        : Expr(ExprKind::INTEGER_LITERAL, range),
          value_(value) {}

void IntegerLiteralExpr::accept(ASTVisitor &visitor) const {
    visitor.visit(*this);
}

int IntegerLiteralExpr::value() const {
    return value_;
}

} // namespace sodium
