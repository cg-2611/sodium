#ifndef SODIUM_NAC_AST_AST_PRINTER_H
#define SODIUM_NAC_AST_AST_PRINTER_H

#include "sodium/nac/ast/ast_visitor.h"

namespace sodium {

class AST;

class ASTPrinter : public ASTVisitor {
public:
    /**
     * @brief Construct a new ASTPrinter object.
     *
     * @param spaces The width of indentation in spaces.
     */
    ASTPrinter(int spaces);

    /**
     * @brief Destroy the ASTPrinter object.
     *
     */
    ~ASTPrinter() = default;

    /**
     * @brief Print an AST.
     *
     * @param ast The AST to print.
     */
    void printAST(const AST *ast);

    /**
     * @brief Visit a Decl node in the AST. Dispatches the specific visit method for the derived class of Decl.
     *
     * @param decl The Decl node being visited.
     */
    void visit(const Decl *decl) override;

    /**
     * @brief Print a function signature. The visit method of the Identifier, ParameterList and return Type of the
     *        function are called.
     *
     * @param signature The function signature to be printed.
     */
    void visit(const Signature *signature) override;

    /**
     * @brief Print a function declaration. The visit method of the Signature and the Block of the function are called.
     *
     * @param funcDecl The function declaration to be printed.
     */
    void visit(const FuncDecl *funcDecl) override;

    /**
     * @brief Visit an Expr node in the AST. Dispatches the specific visit method for the derived class of Expr.
     *
     * @param expr The Expr node being visited.
     */
    void visit(const Expr *expr) override;

    /**
     * @brief Visit a LiteralExpr node in the AST. Dispatches the specific visit method for the derived class of
     *        LiteralExpr.
     *
     * @param LiteralExpr The LiteralExpr node being visited.
     */
    void visit(const LiteralExpr *LiteralExpr) override;

    /**
     * @brief Prints a numeric literal.
     *
     * @param numericLiteralExpr The numeric literal to be printed.
     */
    void visit(const NumericLiteralExpr *numericLiteralExpr) override;

    /**
     * @brief Prints an identifier.
     *
     * @param identifier The identifier to be printed.
     */
    void visit(const Identifier *identifier) override;

    /**
     * @brief Prints parameter.
     *
     * @param parameter The parameter to be printed.
     */
    void visit(const Parameter *parameter) override;

    /**
     * @brief Prints a ParameterList. The visit method of each Parameter in the list is called.
     *
     * @param parameterList The ParameterList to be printed.
     */
    void visit(const ParameterList *parameterList) override;

    /**
     * @brief Prints a SourceFile. The visit method of each Decl is called.
     *
     * @param sourceFile The SourceFile node to be printed.
     */
    void visit(const SourceFile *sourceFile) override;

    /**
     * @brief Visit an Stmt node in the AST. Dispatches the specific visit method for the derived class of Stmt.
     *
     * @param stmt The Stmt node being visited.
     */
    void visit(const Stmt *stmt) override;

    /**
     * @brief Prints a Block. The visit method of each Stmt in the block is called.
     *
     * @param block The Block to be printed.
     */
    void visit(const Block *block) override;

    /**
     * @brief Prints a ReturnStmt. The visit method of the returned expression is called.
     *
     * @param returnStmt The ReturnStmt to be printed.
     */
    void visit(const ReturnStmt *returnStmt) override;

    /**
     * @brief Prints a Type.
     *
     * @param type The Type to be printed.
     */
    void visit(const Type *type) override;

private:
    int indentationSpaces_;
    int indentationLevel_;

    void printIndentation(); // print the correct number of spaces of indentation
    void indent();           // increases the indentation level by 1
    void dedent();           // decreases the indentation level by 1
};

} // namespace sodium

#endif // SODIUM_NAC_AST_AST_PRINTER_H
