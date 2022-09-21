#ifndef SODIUM_NAC_AST_AST_H
#define SODIUM_NAC_AST_AST_H

#include <memory>

namespace sodium {

class ASTNode;

/// Used to represent the AST of a Sodium file.
class AST {
public:
    /// Constructor for AST.
    /// \param root An ASTNode that represents the root of the AST.
    AST(std::unique_ptr<ASTNode> root);

    /// \return The root node of \c this AST.
    [[nodiscard]] ASTNode *root() const;

private:
    std::unique_ptr<ASTNode> root_;
};

} // namespace sodium

#endif // SODIUM_NAC_AST_AST_H
