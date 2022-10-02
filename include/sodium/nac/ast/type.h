#ifndef SODIUM_NAC_AST_TYPE_H
#define SODIUM_NAC_AST_TYPE_H

#include <string>

#include "sodium/nac/ast/ast_node.h"

namespace llvm {

class Value;

} // namespace llvm

namespace sodium {

class ASTVisitor;
class CodegenVisitor;
class SourceRange;

/// A derived class of ASTNode, used to represent a type in Sodium.
class Type : public ASTNode {
public:
    /// Constructor for Type.
    /// \param name The name of the type.
    /// \param range The range of the type in the source code.
    Type(std::string name, SourceRange range);

    /// Accepts an ASTVisitor for \c this type.
    /// \param visitor The ASTVisitor being accepted by \c this.
    void accept(ASTVisitor &visitor) const override;

    llvm::Value *accept(CodegenVisitor &visitor) const override;

    /// \return The name of \c this type.
    [[nodiscard]] const std::string &name() const;

private:
    std::string name_;
};

} // namespace sodium

#endif // SODIUM_NAC_AST_TYPE_H
