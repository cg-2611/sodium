#ifndef SODIUM_NAC_AST_STMT_H
#define SODIUM_NAC_AST_STMT_H

#include <memory>
#include <vector>

#include "sodium/nac/ast/ast_node.h"

namespace sodium {

class ASTVisitor;
class Expr;
class SourceRange;

/// An enum of the different kinds of statements in Sodium.
enum class StmtKind {
    BLOCK,
    RETURN
};

/// A base class for any statement in Sodium.
class Stmt : public ASTNode {
public:
    Stmt(const Stmt &) = delete;
    Stmt &operator=(const Stmt &) = delete;

    Stmt &operator=(Stmt &&) noexcept = delete;
    Stmt(Stmt &&) noexcept = delete;

    ~Stmt() override = default;

    /// \return The kind of \c this statement.
    [[nodiscard]] StmtKind stmt_kind() const;

protected:
    /// Constructor for Stmt.
    /// \param kind The kind of the statement.
    /// \param range The range of the statement in the source code.
    Stmt(StmtKind kind, SourceRange range);

private:
    StmtKind kind_;
};

/// A derived class of Stmt, used to represent a block of statements in Sodium.
class Block : public Stmt {
public:
    /// Constructor for Block.
    /// \param stmts The statements in the block.
    /// \param range The range of the start of the block in the source code.
    Block(std::vector<std::unique_ptr<Stmt>> stmts, SourceRange range);

    /// Accepts an ASTVisitor for \c this block.
    /// \param visitor The ASTVisitor being accepted by \c this.
    void accept(ASTVisitor &visitor) const override;

    /// \return The  statements in \c this block.
    [[nodiscard]] const std::vector<std::unique_ptr<Stmt>> &stmts() const;

private:
    std::vector<std::unique_ptr<Stmt>> stmts_;
};

/// A derived class of Stmt, used to represent a return statement in Sodium.
class ReturnStmt : public Stmt {
public:
    /// Constructor for ReturnStmt.
    /// \param expr The expression being returned by the statement.
    /// \param range The range of the end of the return statement in the source code.
    ReturnStmt(std::unique_ptr<Expr> expr, SourceRange range);

    /// Accepts an ASTVisitor for \c this return statement.
    /// \param visitor The ASTVisitor being accepted by \c this.
    void accept(ASTVisitor &visitor) const override;

    /// \return The expression being returned by \c this statement.
    [[nodiscard]] Expr *expr() const;

private:
    std::unique_ptr<Expr> expr_;
};

} // namespace sodium

#endif // SODIUM_NAC_AST_STMT_H
