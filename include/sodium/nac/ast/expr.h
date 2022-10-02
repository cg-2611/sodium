#ifndef SODIUM_NAC_AST_EXPR_H
#define SODIUM_NAC_AST_EXPR_H

#include "sodium/nac/ast/ast_node.h"

namespace llvm {

class Value;

} // namespace llvm

namespace sodium {

class ASTVisitor;
class CodegenVisitor;
class SourceRange;

/// An enum of the different kinds of expressions in Sodium.
enum class ExprKind {
    INTEGER_LITERAL
};

/// A base class for any expression in Sodium.
class Expr : public ASTNode {
public:
    Expr(const Expr &) = delete;
    Expr &operator=(const Expr &) = delete;

    Expr(Expr &&) noexcept = delete;
    Expr &operator=(Expr &&) noexcept = delete;

    ~Expr() override = default;

    /// \return The kind of \c this expression.
    [[nodiscard]] ExprKind expr_kind() const;

protected:
    /// Constructor for Expr.
    /// \param kind The kind of the expression.
    /// \param range The range of the expression in the source code.
    Expr(ExprKind kind, SourceRange range);

private:
    ExprKind kind_;
};

/// A derived class of Expr, used to represent an integer literal in Sodium.
class IntegerLiteralExpr : public Expr {
public:
    /// Constructor for IntegerLiteralExpr.
    /// \param value The integer value of the integer literal.
    /// \param range The range of the integer literal expression in the source code.
    IntegerLiteralExpr(int value, SourceRange range);

    /// Accepts an ASTVisitor for \c this integer literal expression.
    /// \param visitor The ASTVisitor being accepted by \c this.
    void accept(ASTVisitor &visitor) const override;

    llvm::Value *accept(CodegenVisitor &visitor) const override;

    /// \return The integer value of \c this integer literal.
    [[nodiscard]] int value() const;

private:
    int value_;
};

} // namespace sodium

#endif // SODIUM_NAC_AST_EXPR_H
