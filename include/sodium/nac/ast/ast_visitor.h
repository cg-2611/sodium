#ifndef SODIUM_NAC_AST_AST_VISITOR_H
#define SODIUM_NAC_AST_AST_VISITOR_H

namespace sodium {

class SourceFile;
class Decl;
class FuncDecl;
class FunctionSignature;
class ParameterList;
class Stmt;
class ReturnStmt;
class Block;
class Expr;
class LiteralExpr;
class NumericLiteralExpr;
class Identifier;
class Type;

/**
 * @brief An interface that can be inherited and used to create visitor objects for use with the visitor pattern.
 *
 */
class ASTVisitor {
public:
    /**
     * @brief Destroy the ASTVisitor object.
     *
     */
    virtual ~ASTVisitor() = default;

    /**
     * @brief Pure virtual method. Must be overriden in derived classes.
     *        Used to visit a SourceFile AST node.
     *
     * @param sourceFile The SourceFile being visited.
     */
    virtual void visit(const SourceFile *sourceFile) = 0;

    /**
     * @brief Pure virtual method. Must be overriden in derived classes.
     *        Used to visit a Decl AST node.
     *
     * @param decl The Decl being visited.
     */
    virtual void visit(const Decl *decl) = 0;

    /**
     * @brief Pure virtual method. Must be overriden in derived classes.
     *        Used to visit a FuncDecl AST node.
     *
     * @param funcDecl The FuncDecl being visited.
     */
    virtual void visit(const FuncDecl *funcDecl) = 0;

    /**
     * @brief Pure virtual method. Must be overriden in derived classes.
     *        Used to visit a FunctionSignature AST node.
     *
     * @param functionSignature The FunctionSignature being visited.
     */
    virtual void visit(const FunctionSignature *functionSignature) = 0;

    /**
     * @brief Pure virtual method. Must be overriden in derived classes.
     *        Used to visit a ParameterList AST node.
     *
     * @param parameterList The ParameterList being visited.
     */
    virtual void visit(const ParameterList *parameterList) = 0;

    /**
     * @brief Pure virtual method. Must be overriden in derived classes.
     *        Used to visit a Stmt AST node.
     *
     * @param stmt The Stmt being visited.
     */
    virtual void visit(const Stmt *stmt) = 0;

    /**
     * @brief Pure virtual method. Must be overriden in derived classes.
     *        Used to visit a Block AST node.
     *
     * @param block The Block being visited.
     */
    virtual void visit(const Block *block) = 0;

    /**
     * @brief Pure virtual method. Must be overriden in derived classes.
     *        Used to visit a ReturnStmt AST node.
     *
     * @param returnStmt The ReturnStmt being visited.
     */
    virtual void visit(const ReturnStmt *returnStmt) = 0;

    /**
     * @brief Pure virtual method. Must be overriden in derived classes.
     *        Used to visit a Expr AST node.
     *
     * @param expr The Expr being visited.
     */
    virtual void visit(const Expr *expr) = 0;

    /**
     * @brief Pure virtual method. Must be overriden in derived classes.
     *        Used to visit a LiteralExpr AST node.
     *
     * @param literalExpr The LiteralExpr being visited.
     */
    virtual void visit(const LiteralExpr *literalExpr) = 0;

    /**
     * @brief Pure virtual method. Must be overriden in derived classes.
     *        Used to visit a NumericLiteralExpr AST node.
     *
     * @param numericLiteralExpr The numericLiteralExpr being visited.
     */
    virtual void visit(const NumericLiteralExpr *numericLiteralExpr) = 0;

    /**
     * @brief Pure virtual method. Must be overriden in derived classes.
     *        Used to visit a Identifier AST node.
     *
     * @param identifier The Identifier being visited.
     */
    virtual void visit(const Identifier *identifier) = 0;

    /**
     * @brief Pure virtual method. Must be overriden in derived classes.
     *        Used to visit a Type AST node.
     *
     * @param type The Type being visited.
     */
    virtual void visit(const Type *type) = 0;

protected:
    /**
     * @brief Construct a new ASTVisitor object.
     *
     */
    ASTVisitor() = default;

    /**
     * @brief Construct a new ASTVisitor object by copying another ASTVisitor.
     *
     * @param other The other ASTVisitor being copied to construct this one.
     */
    ASTVisitor(const ASTVisitor &other) = default;

    /**
     * @brief Construct a new ASTVisitor object by moving another ASTVisitor.
     *
     * @param other The other ASTVisitor being moved to construct this one,
     */
    ASTVisitor(ASTVisitor &&other) noexcept = default;
    /**
     * @brief Copy assignment operator.
     *
     * @param other The other ASTVisitor being copied.
     * @return ASTVisitor& that is \c this ASTVisitor after the assignment
     */
    ASTVisitor &operator=(const ASTVisitor &other) = default;

    /**
     * @brief Move assignment operator.
     *
     * @param other The other ASTVisitor being moved.
     * @return ASTVisitor& that is \c this ASTVisitor after the assignment.
     */
    ASTVisitor &operator=(ASTVisitor &&other) noexcept = default;
};

} // namespace sodium

#endif // SODIUM_NAC_AST_AST_VISITOR_H
