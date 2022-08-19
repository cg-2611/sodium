#ifndef SODIUM_NAC_AST_AST_NODE_H
#define SODIUM_NAC_AST_AST_NODE_H

namespace sodium {

/**
 * @brief An enum of the different kinds of AST nodes.
 *
 */
enum class ASTNodeKind {
    SOURCE_FILE,
    DECL,
    EXPR,
    STMT,
    IDENTIFIER,
    TYPE
};

class ASTVisitor;

/**
 * @brief A base class for all nodes in the AST.
 *
 */
class ASTNode {
public:
    /**
     * @brief Construct a new ASTNode object.
     *
     * @param kind The kind of the node, must be one of ASTNodeKind.
     */
    ASTNode(ASTNodeKind kind);

    /**
     * @brief Destroy the ASTNode object.
     *
     */
    virtual ~ASTNode() = default;

    /**
     * @brief Pure virtual method. Must be overridden in derived classes.
     *        Used to accept an ASTVisitor derived class for use with the visitor pattern.
     *
     * @param visitor The visitor object being accepted by this node.
     */
    virtual void accept(ASTVisitor *visitor) const = 0;

    /**
     * @brief Access the kind of this AST node.
     *
     * @return ASTNodeKind that is the kind of this node.
     */
    ASTNodeKind nodeKind() const noexcept;

private:
    ASTNodeKind kind_;
};

} // namespace sodium

#endif // SODIUM_NAC_AST_AST_NODE_H
