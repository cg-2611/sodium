#ifndef SODIUM_NAC_AST_EXPR_H
#define SODIUM_NAC_AST_EXPR_H

#include "sodium/nac/ast/ast_node.h"

namespace sodium {

class ASTVisitor;

/**
 * @brief An enum of the different kinds of expressions in Sodium.
 *
 */
enum class ExprKind {
    LITERAL
};

/**
 * @brief A derived class of ASTNode, and a base class for any expression in Sodium.
 *
 */
class Expr : public ASTNode {
public:
    /**
     * @brief Construct a new Expr object.
     *
     * @param kind The kind of expression, must be one of ExprKind.
     */
    Expr(ExprKind kind);

    /**
     * @brief Destroy the Expr object.
     *
     */
    virtual ~Expr() = default;

    /**
     * @brief Used to accept, for an expression, an ASTVisitor derived class for use with the visitor pattern.
     *        It is also overridden in derived classes of this class.
     *
     * @param visitor The visitor object being accepted by this expression.
     */
    virtual void accept(ASTVisitor *visitor) const override;

    /**
     * @brief Accessor for the kind of this expression.
     *
     * @return ExprKind that is the kind of this expression.
     */
    ExprKind exprKind() const noexcept;

protected:
    ExprKind kind_;
};

/**
 * @brief An enum of the different kind of literal expressions in Sodium.
 *
 */
enum class LiteralKind {
    NUMERIC_LITERAL
};

/**
 * @brief A derived class of Expr and a base class for all literal expressions in Sodium.
 *
 */
class LiteralExpr : public Expr {
public:
    /**
     * @brief Construct a new LiteralExpr object.
     *
     * @param kind The kind of literal expression, must be one of LiteralKind.
     */
    LiteralExpr(LiteralKind kind);

    /**
     * @brief Destroy the LiteralExpr object.
     *
     */
    virtual ~LiteralExpr() = default;

    /**
     * @brief Used to accept, for a literal expression, an ASTVisitor derived class for use with the visitor pattern.
     *        It is also overridden in derived classes of this class.
     *
     * @param visitor The visitor object being accepted by this literal expression.
     */
    virtual void accept(ASTVisitor *visitor) const override;

    /**
     * @brief Accessor for the kind of this literal expression.
     *
     * @return LiteralKind that is the kind of this literal expression.
     */
    LiteralKind literalKind() const noexcept;

protected:
    LiteralKind kind_;
};

/**
 * @brief A derived class of LiteralExpr, used to represent a numeric literal in Sodium.
 *
 */
class NumericLiteralExpr : public LiteralExpr {
public:
    /**
     * @brief Construct a new NumericLiteralExpr object.
     *
     * @param value The integer value of the numeric literal.
     */
    NumericLiteralExpr(int value);

    /**
     * @brief Destroy the NumericLiteralExpr object.
     *
     */
    ~NumericLiteralExpr() = default;

    /**
     * @brief Used to accept, for a numeric literal expression, an ASTVisitor derived class for use with the visitor
     *        pattern.
     *
     * @param visitor The visitor object being accepted by this numeric literal.
     */
    void accept(ASTVisitor *visitor) const override;

    /**
     * @brief Accessor for the integer value of this numeric literal.
     *
     * @return int that is the integer value of the numeric literal.
     */
    int value() const noexcept;

private:
    int value_;
};

} // namespace sodium

#endif // SODIUM_NAC_AST_EXPR_H
