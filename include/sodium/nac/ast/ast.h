#ifndef SODIUM_NAC_AST_AST_H
#define SODIUM_NAC_AST_AST_H

#include <memory>

namespace sodium {

class ASTNode;

/**
 * @brief Used to represent the AST of a Sodium file.
 *
 */
class AST {
public:
    /**
     * @brief Construct a new AST object.
     *
     * @param root An std::unique_ptr<ASTNode> that represents the root of the AST.
     */
    AST(std::unique_ptr<ASTNode> root);

    /**
     * @brief Access the root of the AST.
     *
     * @return ASTNode* to the root node of the AST.
     */
    ASTNode *root() const noexcept;

private:
    std::unique_ptr<ASTNode> root_;
};

} // namespace sodium

#endif // SODIUM_NAC_AST_AST_H
