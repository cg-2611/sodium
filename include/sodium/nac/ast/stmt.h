#ifndef SODIUM_NAC_AST_STMT_H
#define SODIUM_NAC_AST_STMT_H

#include <memory>
#include <vector>

#include "sodium/nac/ast/ast_node.h"

namespace sodium {

class ASTVisitor;
class Expr;

/**
 * @brief An enum of the different kinds of statements in Sodium.
 *
 */
enum class StmtKind {
    BLOCK,
    RETURN
};

/**
 * @brief A derived class of ASTNode, and a base class for any statement in Sodium.
 *
 */
class Stmt : public ASTNode {
public:
    /**
     * @brief Construct a new Stmt object.
     *
     * @param kind The kind of statement, must be one of StmtKind.
     */
    Stmt(StmtKind kind);

    /**
     * @brief Construct a new Stmt object by copying another Stmt.
     *
     * @param other The other Stmt being copied to construct this one.
     */
    Stmt(const Stmt &other) = default;

    /**
     * @brief Construct a new Stmt object by moving another Stmt.
     *
     * @param other The other Stmt being moved to construct this one,
     */
    Stmt(Stmt &&other) noexcept = default;

    /**
     * @brief Destroy the Stmt object.
     *
     */
    ~Stmt() override = default;

    /**
     * @brief Copy assignment operator.
     *
     * @param other The other Stmt being copied.
     * @return Stmt& that is \c this Stmt after the assignment
     */
    Stmt &operator=(const Stmt &other) = default;

    /**
     * @brief Move assignment operator.
     *
     * @param other The other Stmt being moved.
     * @return Stmt& that is \c this Stmt after the assignment.
     */
    Stmt &operator=(Stmt &&other) noexcept = default;

    /**
     * @brief Used to accept, for a statement, an ASTVisitor derived class for use with the visitor pattern.
     *        It is also overridden in derived classes of this class.
     *
     * @param visitor The visitor object being accepted by this expression.
     */
    void accept(ASTVisitor *visitor) const override;

    /**
     * @brief Accessor for the kind of this statement.
     *
     * @return StmtKind that is the kind of this statement.
     */
    StmtKind stmtKind() const noexcept;

private:
    StmtKind kind_;
};

/**
 * @brief A derived class of Stmt, used to represent a block of statements in Sodium.
 *
 */
class Block : public Stmt {
public:
    /**
     * @brief Construct a new Block object.
     *
     * @param stmts An std::vector<std::unique_ptr<Stmt> that are the statements in the block.
     */
    Block(std::vector<std::unique_ptr<Stmt>> stmts);

    /**
     * @brief Used to accept, for a block, an ASTVisitor derived class for use with the visitor pattern.
     *        It calls the accept method of each statement in the vector of statements.
     *
     * @param visitor The visitor object being accepted by this block.
     */
    void accept(ASTVisitor *visitor) const override;

    /**
     * @brief Accessor for the list of declarations in the source file.
     *
     * @return const std::vector<std::unique_ptr<Stmt>>& that stores the statements in the block.
     */
    const std::vector<std::unique_ptr<Stmt>> &stmts() const noexcept;

private:
    std::vector<std::unique_ptr<Stmt>> stmts_;
};

/**
 * @brief A derived class of Stmt, used to represent a return statement in Sodium.
 *
 */
class ReturnStmt : public Stmt {
public:
    /**
     * @brief Construct a new ReturnStmt object.
     *
     * @param expr An std::unique_ptr<Expr> that is the expression being returned by the statement.
     */
    ReturnStmt(std::unique_ptr<Expr> expr);

    /**
     * @brief Used to accept, for a return statement, an ASTVisitor derived class for use with the visitor pattern.
     *        It class the accept method of the expression too.
     *
     * @param visitor The visitor object being accepted by this return statement.
     */
    void accept(ASTVisitor *visitor) const override;

    /**
     * @brief Accessor for the expression being returned by this statement.
     *
     * @return Expr* that is the expression being returned by this statement.
     */
    Expr *expr() const noexcept;

private:
    std::unique_ptr<Expr> expr_;
};

} // namespace sodium

#endif // SODIUM_NAC_AST_STMT_H
