#ifndef SODIUM_NAC_AST_DECL_H
#define SODIUM_NAC_AST_DECL_H

#include <memory>

#include "sodium/nac/ast/ast_node.h"

namespace llvm {

class Value;

} // namespace llvm

namespace sodium {

class ASTVisitor;
class Block;
class CodegenVisitor;
class Identifier;
class SourceRange;
class Type;

/// An enum of the different kinds of declarations that can be made in Sodium.
enum class DeclKind {
    FUNC
};

/// A base class for any declaration that can be made in Sodium.
class Decl : public ASTNode {
public:
    Decl(const Decl &other) = delete;
    Decl &operator=(const Decl &other) = delete;

    Decl(Decl &&other) noexcept = delete;
    Decl &operator=(Decl &&other) noexcept = delete;

    ~Decl() override = default;

    /// \return The kind of \c this declaration.
    [[nodiscard]] DeclKind decl_kind() const;

protected:
    /// Constructor for Decl.
    /// \param kind The kind of the declaration.
    /// \param range The range of the declaration in the source code.
    Decl(DeclKind kind, SourceRange range);

private:
    DeclKind kind_;
};

/// A derived class of Decl, used to represent a function declaration in Sodium.
class FuncDecl : public Decl {
public:
    /// Constructor for FuncDecl.
    /// \param name The name of the function declaration.
    /// \param return_type The return type of the function declaration.
    /// \param body The body of the function declaration.
    /// \param range The range of the function declaration in the source code.
    FuncDecl(std::unique_ptr<Identifier> name, std::unique_ptr<Type> return_type, std::unique_ptr<Block> body,
             SourceRange range);

    /// Accepts an ASTVisitor for \c this function declaration.
    /// \param visitor The ASTVisitor being accepted by \c this.
    void accept(ASTVisitor &visitor) const override;

    llvm::Value *accept(CodegenVisitor &visitor) const override;

    /// \return The name of \c this function declaration.
    [[nodiscard]] Identifier *name() const;

    /// \return The return type of \c this function declaration.
    [[nodiscard]] Type *return_type() const;

    /// \return The body of \c this function declaration.
    [[nodiscard]] Block *body() const;

private:
    std::unique_ptr<Identifier> name_;
    std::unique_ptr<Type> return_type_;
    std::unique_ptr<Block> body_;
};

} // namespace sodium

#endif // SODIUM_NAC_AST_DECL_H
