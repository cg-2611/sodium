#ifndef SODIUM_NAC_AST_IDENTIFIER_H
#define SODIUM_NAC_AST_IDENTIFIER_H

#include <string_view>

#include "sodium/nac/ast/ast_node.h"

namespace sodium {

class ASTVisitor;

/**
 * @brief A derived class of ASTNode, used to represent an identifier in Sodium.
 *
 */
class Identifier : public ASTNode {
public:
    /**
     * @brief Construct a new Identifier object.
     *
     * @param value An std::string_view of the value of the identifier.
     */
    Identifier(std::string_view value);

    /**
     * @brief Destroy the Identifier object.
     *
     */
    ~Identifier() = default;

    /**
     * @brief Used to accept, for an identifier, an ASTVisitor derived class for use with the visitor pattern.
     *
     * @param visitor The visitor object being accepted by this identifier.
     */
    void accept(ASTVisitor *visitor) const override;

    /**
     * @brief Accessor for the value of this identifier.
     *
     * @return std::string_view that is the value of this identifier.
     */
    std::string_view value() const noexcept;

private:
    std::string_view value_;
};

} // namespace sodium

#endif // SODIUM_NAC_AST_IDENTIFIER_H
