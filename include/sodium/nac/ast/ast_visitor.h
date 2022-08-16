#ifndef SODIUM_NAC_AST_AST_VISITOR_H
#define SODIUM_NAC_AST_AST_VISITOR_H

namespace sodium {

class Decl;
class Signature;
class FuncDecl;
class Expr;
class LiteralExpr;
class NumericLiteralExpr;
class Identifier;
class Parameter;
class ParameterList;
class SourceFile;
class Stmt;
class Block;
class ReturnStmt;
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
     *        Used to visit a Decl AST node.
     *
     * @param decl The Decl being visited.
     */
    virtual void visit(const Decl *decl) = 0;

    /**
     * @brief Pure virtual method. Must be overriden in derived classes.
     *        Used to visit a Signature AST node.
     *
     * @param signature The Signature being visited.
     */
    virtual void visit(const Signature *signature) = 0;

    /**
     * @brief Pure virtual method. Must be overriden in derived classes.
     *        Used to visit a FuncDecl AST node.
     *
     * @param funcDecl The FuncDecl being visited.
     */
    virtual void visit(const FuncDecl *funcDecl) = 0;

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
     * @param LiteralExpr The LiteralExpr being visited.
     */
    virtual void visit(const LiteralExpr *LiteralExpr) = 0;

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
     *        Used to visit a Parameter AST node.
     *
     * @param parameter The Parameter being visited.
     */
    virtual void visit(const Parameter *parameter) = 0;

    /**
     * @brief Pure virtual method. Must be overriden in derived classes.
     *        Used to visit a ParameterList AST node.
     *
     * @param parameterList The ParameterList being visited.
     */
    virtual void visit(const ParameterList *parameterList) = 0;

    /**
     * @brief Pure virtual method. Must be overriden in derived classes.
     *        Used to visit a SourceFile AST node.
     *
     * @param sourceFile The SourceFile being visited.
     */
    virtual void visit(const SourceFile *sourceFile) = 0;

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
};

} // namespace sodium

#endif // SODIUM_NAC_AST_AST_VISITOR_H
