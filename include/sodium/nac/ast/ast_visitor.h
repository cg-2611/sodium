#ifndef SODIUM_NAC_AST_AST_VISITOR_H
#define SODIUM_NAC_AST_AST_VISITOR_H

namespace sodium {

class Block;
class FuncDecl;
class FuncSignature;
class Identifier;
class IntegerLiteralExpr;
class ParameterList;
class ReturnStmt;
class SourceFile;
class Type;

/// An interface for the visitor pattern to traverse an AST.
class ASTVisitor {
public:
    ASTVisitor(const ASTVisitor &) = delete;
    ASTVisitor &operator=(const ASTVisitor &) = delete;

    ASTVisitor(ASTVisitor &&) noexcept = delete;
    ASTVisitor &operator=(ASTVisitor &&) noexcept = delete;

    virtual ~ASTVisitor() = default;

    /// Visit a SourceFile AST node.
    /// \param source_file The SourceFile being visited.
    virtual void visit(const SourceFile &source_file) = 0;

    /// Visit a FuncDecl AST node.
    /// \param func_decl The FuncDecl being visited.
    virtual void visit(const FuncDecl &func_decl) = 0;

    /// Visit a FuncSignature AST node.
    /// \param function_signature The FuncSignature being visited.
    virtual void visit(const FuncSignature &function_signature) = 0;

    /// Visit a ParameterList AST node.
    /// \param parameter_list The ParameterList being visited.
    virtual void visit(const ParameterList &parameter_list) = 0;

    /// Visit a Block AST node.
    /// \param block The Block being visited.
    virtual void visit(const Block &block) = 0;

    /// Visit a ReturnStmt AST node.
    /// \param return_stmt The ReturnStmt being visited.
    virtual void visit(const ReturnStmt &return_stmt) = 0;

    /// Visit a IntegerLiteralExpr AST node.
    /// \param integer_literal_expr The numericLiteralExpr being visited.
    virtual void visit(const IntegerLiteralExpr &integer_literal_expr) = 0;

    /// Visit a Identifier AST node.
    /// \param identifier The Identifier being visited.
    virtual void visit(const Identifier &identifier) = 0;

    /// Visit a Type AST node.
    /// \param type The Type being visited.
    virtual void visit(const Type &type) = 0;

protected:
    ASTVisitor() = default;
};

} // namespace sodium

#endif // SODIUM_NAC_AST_AST_VISITOR_H
