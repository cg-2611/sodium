#ifndef SODIUM_NAC_AST_PARAMETER_LIST_H
#define SODIUM_NAC_AST_PARAMETER_LIST_H

#include <memory>
#include <vector>

#include "sodium/nac/ast/ast_node.h"

namespace sodium {

class ASTVisitor;
class Identifier;

/**
 * @brief A derived class of ASTNode, used to represent a single function parameter in Sodium.
 *
 */
class Parameter : public ASTNode {
public:
    /**
     * @brief Construct a new Parameter object.
     *
     * @param identifier An std::unique_ptr<Identifier> that is the name of the parameter.
     */
    Parameter(std::unique_ptr<Identifier> identifier);

    /**
     * @brief Destroy the Parameter object.
     *
     */
    ~Parameter() = default;

    /**
     * @brief Used to accept, for a parameter, an ASTVisitor derived class for use with the visitor pattern.
     *
     * @param visitor The visitor object being accepted by this parameter.
     */
    void accept(ASTVisitor *visitor) const override;

    /**
     * @brief Accessor for the identifier of the parameter.
     *
     * @return Identifier* that stores the name of the parameter.
     */
    Identifier *identifier() const noexcept;

private:
    std::unique_ptr<Identifier> identifier_;
};

/**
 * @brief A derived class of ASTNode, used to represent a list of function parameter in Sodium.
 *
 */
class ParameterList : public ASTNode {
public:
    /**
     * @brief Construct a new ParameterList object.
     *
     * @param parameters An std::vector<std::unique_ptr<Parameter>> that is the list of parameters.
     */
    ParameterList(std::vector<std::unique_ptr<Parameter>> parameters);

    /**
     * @brief Destroy the ParameterList object.
     *
     */
    ~ParameterList() = default;

    /**
     * @brief Used to accept, for a parameter list, an ASTVisitor derived class for use with the visitor pattern.
     *        It calls the accept method of each parameter in the vector of parameters.
     *
     * @param visitor The visitor object being accepted by this parameter list.
     */
    void accept(ASTVisitor *visitor) const override;

    /**
     * @brief Accessor for the list of parameters in the parameter list.
     *
     * @return const std::vector<std::unique_ptr<Parameter>>& that stores the parameters in the parameter list.
     */
    const std::vector<std::unique_ptr<Parameter>> &parameters() const noexcept;

private:
    std::vector<std::unique_ptr<Parameter>> parameters_;
};

} // namespace sodium

#endif // SODIUM_NAC_AST_PARAMETER_LIST_H
