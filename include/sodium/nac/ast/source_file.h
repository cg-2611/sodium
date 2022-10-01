#ifndef SODIUM_NAC_AST_SOURCE_FILE_H
#define SODIUM_NAC_AST_SOURCE_FILE_H

#include <memory>
#include <vector>

#include "sodium/nac/ast/ast_node.h"

namespace llvm {

class Value;

} // namespace llvm

namespace sodium {

class ASTVisitor;
class CodegenVisitor;
class Decl;
class SourceRange;

/// A derived class of ASTNode, used to represent a Sodium source file.
class SourceFile : public ASTNode {
public:
    /// Constructor for SourceFile.
    /// \param decls The declarations in the source file.
    /// \param range The range of the contents of the source code.
    SourceFile(std::vector<std::unique_ptr<Decl>> decls, SourceRange range);

    /// Accepts an ASTVisitor for \c this source file.
    /// \param visitor The ASTVisitor being accepted by \c this.
    void accept(ASTVisitor &visitor) const override;

    llvm::Value *accept(CodegenVisitor &visitor) const override;

    /// \return The declarations in \c this source file.
    [[nodiscard]] const std::vector<std::unique_ptr<Decl>> &decls() const;

private:
    std::vector<std::unique_ptr<Decl>> decls_;
};

} // namespace sodium

#endif // SODIUM_NAC_AST_SOURCE_FILE_H
