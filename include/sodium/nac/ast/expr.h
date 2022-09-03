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
     * @brief Construct a new Expr object by copying another Expr.
     *
     * @param other The other Expr being copied to construct this one.
     */
    Expr(const Expr &other) = default;

    /**
     * @brief Construct a new Expr object by moving another Expr.
     *
     * @param other The other Expr being moved to construct this one,
     */
    Expr(Expr &&other) noexcept = default;

    /**
     * @brief Destroy the Expr object.
     *
     */
    ~Expr() override = default;

    /**
     * @brief Copy assignment operator.
     *
     * @param other The other Expr being copied.
     * @return Expr& that is \c this Expr after the assignment
     */
    Expr &operator=(const Expr &other) = default;

    /**
     * @brief Move assignment operator.
     *
     * @param other The other Expr being moved.
     * @return Expr& that is \c this Expr after the assignment.
     */
    Expr &operator=(Expr &&other) noexcept = default;

    /**
     * @brief Used to accept, for an expression, an ASTVisitor derived class for use with the visitor pattern.
     *        It is also overridden in derived classes of this class.
     *
     * @param visitor The visitor object being accepted by this expression.
     */
    void accept(ASTVisitor *visitor) const override;

    /**
     * @brief Accessor for the kind of this expression.
     *
     * @return ExprKind that is the kind of this expression.
     */
    ExprKind exprKind() const noexcept;

private:
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
     * @brief Construct a new LiteralExpr object by copying another LiteralExpr.
     *
     * @param other The other LiteralExpr being copied to construct this one.
     */
    LiteralExpr(const LiteralExpr &other) = default;

    /**
     * @brief Construct a new LiteralExpr object by moving another LiteralExpr.
     *
     * @param other The other LiteralExpr being moved to construct this one,
     */
    LiteralExpr(LiteralExpr &&other) noexcept = default;

    /**
     * @brief Destroy the LiteralExpr object.
     *
     */
    ~LiteralExpr() override = default;

    /**
     * @brief Copy assignment operator.
     *
     * @param other The other LiteralExpr being copied.
     * @return LiteralExpr& that is \c this LiteralExpr after the assignment
     */
    LiteralExpr &operator=(const LiteralExpr &other) = default;

    /**
     * @brief Move assignment operator.
     *
     * @param other The other LiteralExpr being moved.
     * @return LiteralExpr& that is \c this LiteralExpr after the assignment.
     */
    LiteralExpr &operator=(LiteralExpr &&other) noexcept = default;

    /**
     * @brief Used to accept, for a literal expression, an ASTVisitor derived class for use with the visitor pattern.
     *        It is also overridden in derived classes of this class.
     *
     * @param visitor The visitor object being accepted by this literal expression.
     */
    void accept(ASTVisitor *visitor) const override;

    /**
     * @brief Accessor for the kind of this literal expression.
     *
     * @return LiteralKind that is the kind of this literal expression.
     */
    LiteralKind literalKind() const noexcept;

private:
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
