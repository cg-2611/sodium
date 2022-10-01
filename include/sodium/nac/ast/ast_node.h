#ifndef SODIUM_NAC_AST_AST_NODE_H
#define SODIUM_NAC_AST_AST_NODE_H

#include "sodium/nac/basic/source_range.h"

namespace llvm {

class Value;

} // namespace llvm

namespace sodium {

class ASTVisitor;
class CodegenVisitor;

/// An enum of the different kinds of AST nodes.
enum class ASTNodeKind {
    DECL,
    EXPR,
    IDENTIFIER,
    STMT,
    SOURCE_FILE,
    TYPE
};

/// A base class for all nodes in the AST.
class ASTNode {
public:
    ASTNode(const ASTNode &) = default;
    ASTNode &operator=(const ASTNode &) = default;

    ASTNode(ASTNode &&) noexcept = default;
    ASTNode &operator=(ASTNode &&) noexcept = default;

    virtual ~ASTNode() = default;

    /// Accepts an ASTVisitor for \c a derived AST node.
    /// \param visitor The ASTVisitor being accepted by \c the derived AST node.
    virtual void accept(ASTVisitor &visitor) const = 0;

    virtual llvm::Value *accept(CodegenVisitor &visitor) const = 0;

    /// \return The kind of \c this node.
    [[nodiscard]] ASTNodeKind node_kind() const;

    /// \return The range of \c this node in the source code.
    [[nodiscard]] SourceRange range() const;

protected:
    /// Constructor for ASTNode.
    /// \param kind The kind of the node.
    /// \param range The range of the node in the source code.
    ASTNode(ASTNodeKind kind, SourceRange range);

private:
    ASTNodeKind kind_;
    SourceRange range_;
};

} // namespace sodium

#endif // SODIUM_NAC_AST_AST_NODE_H
