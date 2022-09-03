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
     * @brief Construct a new ASTNode object by copying another ASTNode.
     *
     * @param other The other ASTNode being copied to construct this one.
     */
    ASTNode(const ASTNode &other) = default;

    /**
     * @brief Construct a new ASTNode object by moving another ASTNode.
     *
     * @param other The other ASTNode being moved to construct this one,
     */
    ASTNode(ASTNode &&other) noexcept = default;

    /**
     * @brief Destroy the ASTNode object.
     *
     */
    virtual ~ASTNode() = default;

    /**
     * @brief Copy assignment operator.
     *
     * @param other The other ASTNode being copied.
     * @return ASTNode& that is \c this ASTNode after the assignment
     */
    ASTNode &operator=(const ASTNode &other) = default;

    /**
     * @brief Move assignment operator.
     *
     * @param other The other ASTNode being moved.
     * @return ASTNode& that is \c this ASTNode after the assignment.
     */
    ASTNode &operator=(ASTNode &&other) noexcept = default;

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
