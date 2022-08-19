#ifndef SODIUM_NAC_AST_DECL_H
#define SODIUM_NAC_AST_DECL_H

#include <memory>

#include "sodium/nac/ast/ast_node.h"

namespace sodium {

class ASTVisitor;
class Block;
class FunctionSignature;
class Identifier;
class ParameterList;
class Type;

/**
 * @brief An enum of the different kinds of declarations that can be made in Sodium.
 *
 */
enum class DeclKind {
    FUNCTION,
    FUNCTION_SIGNATURE,
    PARAMETER_LIST
};

/**
 * @brief A derived class of ASTNode, and a base class for any declaration that can be made in Sodium.
 *
 */
class Decl : public ASTNode {
public:
    /**
     * @brief Construct a new Decl object.
     *
     * @param kind The kind of declaration, must be one of DeclKind.
     */
    Decl(DeclKind kind);

    /**
     * @brief Destroy the Decl object.
     *
     */
    virtual ~Decl() = default;

    /**
     * @brief Used to accept, for a declaration, an ASTVisitor derived class for use with the visitor pattern.
     *        It is also overridden in derived classes of this class.
     *
     * @param visitor The visitor object being accepted by this declaration.
     */
    virtual void accept(ASTVisitor *visitor) const override;

    /**
     * @brief Accessor for the kind of this declaration.
     *
     * @return DeclKind that is the kind of this declaration.
     */
    DeclKind declKind() const noexcept;

protected:
    DeclKind kind_;
};

/**
 * @brief A derived class of Decl, used to represent a function declaration in Sodium.
 *
 */
class FuncDecl : public Decl {
public:
    /**
     * @brief Construct a new FuncDecl object.
     *
     * @param signature An std::unique_ptr<Signature> that is the function signature.
     * @param body An std::unique_ptr<Block> that is the function body.
     */
    FuncDecl(std::unique_ptr<FunctionSignature> signature, std::unique_ptr<Block> body);

    /**
     * @brief Destroy the FuncDecl object.
     *
     */
    ~FuncDecl() = default;

    /**
     * @brief Used to accept, for a function declaration, an ASTVisitor derived class for use with the visitor
     *        pattern.
     *
     * @param visitor The visitor object being accepted by this function declaration.
     */
    void accept(ASTVisitor *visitor) const override;

    /**
     * @brief Accessor for the function signature.
     *
     * @return Signature* that stores the function signature.
     */
    FunctionSignature *signature() const noexcept;

    /**
     * @brief Acessors for the Block that represents the function body.
     *
     * @return Block* that stores the function body.
     */
    Block *body() const noexcept;

private:
    std::unique_ptr<FunctionSignature> signature_;
    std::unique_ptr<Block> body_;
};

/**
 * @brief Used to represent the signature of a function in Sodium.
 *
 */
class FunctionSignature : public Decl {
public:
    /**
     * @brief Construct a new FunctionSignature object.
     *
     * @param name An std::unique_ptr<Identifier> that is the name of the function.
     * @param parameterList An std::unique_ptr<ParameterList> that are parameter list of the function.
     * @param returnType An std::unique_ptr<Type> that is the return type of the function.
     */
    FunctionSignature(std::unique_ptr<Identifier> name, std::unique_ptr<ParameterList> parameterList,
                      std::unique_ptr<Type> returnType);

    /**
     * @brief Destroy the FunctionSignature object.
     *
     */
    ~FunctionSignature() = default;

    /**
     * @brief Used to accept, for a function signature, an ASTVisitor derived class for use with the visitor
     *        pattern.
     *
     * @param visitor The visitor object being accepted by this function signature.
     */
    void accept(ASTVisitor *visitor) const override;

    /**
     * @brief Accessor for the identifier of the function.
     *
     * @return Identifier* that stores the name of the function.
     */
    Identifier *name() const noexcept;

    /**
     * @brief Accessor for the parameter list of the function.
     *
     * @return ParameterList* that stores the parameter list of the function.
     */
    ParameterList *parameterList() const noexcept;

    /**
     * @brief Accessor for the return type of the function
     *
     * @return Type* that stores the return type of the function.
     */
    Type *returnType() const noexcept;

private:
    std::unique_ptr<Identifier> name_;
    std::unique_ptr<ParameterList> parameterList_;
    std::unique_ptr<Type> returnType_;
};

/**
 * @brief A derived class of ASTNode, used to represent a list of function parameter in Sodium.
 *
 */
class ParameterList : public Decl {
public:
    /**
     * @brief Construct a new ParameterList object.
     *
     */
    ParameterList();

    /**
     * @brief Destroy the ParameterList object.
     *
     */
    ~ParameterList() = default;

    /**
     * @brief Used to accept, for a parameter list, an ASTVisitor derived class for use with the visitor pattern.
     *
     * @param visitor The visitor object being accepted by this parameter list.
     */
    void accept(ASTVisitor *visitor) const override;

private:
};

} // namespace sodium

#endif // SODIUM_NAC_AST_DECL_H
