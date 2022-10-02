#ifndef SODIUM_NAC_AST_IDENTIFIER_H
#define SODIUM_NAC_AST_IDENTIFIER_H

#include <string>

#include "sodium/nac/ast/ast_node.h"

namespace llvm {

class Value;

} // namespace llvm

namespace sodium {

class ASTVisitor;
class CodegenVisitor;
class SourceRange;

/// A derived class of ASTNode, used to represent an identifier in Sodium.
class Identifier : public ASTNode {
public:
    /// Constructor for Identifier.
    /// \param value The value of the identifier.
    /// \param range The range of the identifier in the source code.
    Identifier(std::string value, SourceRange range);

    /// Accepts an ASTVisitor for \c this identifier.
    /// \param visitor The ASTVisitor being accepted by \c this.
    void accept(ASTVisitor &visitor) const override;

    llvm::Value *accept(CodegenVisitor &visitor) const override;

    /// \return The value of \c this identifier.
    [[nodiscard]] const std::string &value() const;

private:
    std::string value_;
};

} // namespace sodium

#endif // SODIUM_NAC_AST_IDENTIFIER_H
