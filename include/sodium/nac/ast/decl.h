#ifndef SODIUM_NAC_AST_DECL_H
#define SODIUM_NAC_AST_DECL_H

#include <memory>

#include "sodium/nac/ast/ast_node.h"

namespace sodium {

class ASTVisitor;
class Block;
class FuncSignature;
class Identifier;
class ParameterList;
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
    /// \param signature The signature of the function declaration..
    /// \param body The body of the function declaration.
    /// \param range The range of the function declaration in the source code.
    FuncDecl(std::unique_ptr<FuncSignature> signature, std::unique_ptr<Block> body, SourceRange range);

    /// Accepts an ASTVisitor for \c this function declaration.
    /// \param visitor The ASTVisitor being accepted by \c this.
    void accept(ASTVisitor &visitor) const override;

    /// \return The signature of \c this function declaration.
    [[nodiscard]] FuncSignature *signature() const;

    /// \return The body of \c this function declaration.
    [[nodiscard]] Block *body() const;

private:
    std::unique_ptr<FuncSignature> signature_;
    std::unique_ptr<Block> body_;
};

/// A derived class of ASTNode, used to represent the signature of a function in Sodium.
class FuncSignature : public ASTNode {
public:
    /// Constructor for FuncSignature.
    /// \param name The name of the function.
    /// \param parameter_list The  parameter list of the function.
    /// \param return_type The return type of the function.
    /// \param range The range of the function signature in the source code.
    FuncSignature(std::unique_ptr<Identifier> name, std::unique_ptr<ParameterList> parameter_list,
                  std::unique_ptr<Type> return_type, SourceRange range);

    /// Accepts an ASTVisitor for \c this function signature.
    /// \param visitor The ASTVisitor being accepted by \c this.
    void accept(ASTVisitor &visitor) const override;

    /// \return The name of \c this function signature.
    [[nodiscard]] Identifier *name() const;

    /// \return The parameter list of \c this function signature.
    [[nodiscard]] ParameterList *parameter_list() const;

    /// \return The return type of \c this function signature.
    [[nodiscard]] Type *return_type() const;

private:
    std::unique_ptr<Identifier> name_;
    std::unique_ptr<ParameterList> parameter_list_;
    std::unique_ptr<Type> return_type_;
};

/// A derived class of ASTNode, used to represent a list of function parameters in Sodium.
class ParameterList : public ASTNode {
public:
    /// Constructor for ParameterList.
    /// \param range The range of of the parameter list in the source code.
    ParameterList(SourceRange range);

    /// Accepts an ASTVisitor for \c this parameter list.
    /// \param visitor The ASTVisitor being accepted by \c this.
    void accept(ASTVisitor &visitor) const override;
};

} // namespace sodium

#endif // SODIUM_NAC_AST_DECL_H
