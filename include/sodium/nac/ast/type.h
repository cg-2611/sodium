#ifndef SODIUM_NAC_AST_TYPE_H
#define SODIUM_NAC_AST_TYPE_H

#include <string_view>

#include "sodium/nac/ast/ast_node.h"

namespace sodium {

class ASTVisitor;

/**
 * @brief A derived class of ASTNode, used to represent a type in Sodium.
 *
 */
class Type : public ASTNode {
public:
    /**
     * @brief Construct a new Type object.
     *
     * @param name An std::string_view of the name of the type.
     */
    Type(std::string_view name);

    /**
     * @brief Used to accept, for a type, an ASTVisitor derived class for use with the visitor pattern.
     *
     * @param visitor The visitor object being accepted by this type.
     */
    void accept(ASTVisitor *visitor) const override;

    /**
     * @brief Accessor for the name of this type.
     *
     * @return std::string_view that is the name of this type.
     */
    std::string_view name() const noexcept;

private:
    std::string_view name_;
};

} // namespace sodium

#endif // SODIUM_NAC_AST_TYPE_H
