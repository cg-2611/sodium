#ifndef SODIUM_NAC_CODEGEN_CODEGEN_VISITOR_H
#define SODIUM_NAC_CODEGEN_CODEGEN_VISITOR_H

namespace llvm {

class Value;

} // namespace llvm

namespace sodium {

class Block;
class FuncDecl;
class Identifier;
class IntegerLiteralExpr;
class ReturnStmt;
class SourceFile;
class Type;

/// An interface for the visitor pattern to traverse an AST and generate the LLVM IR.
class CodegenVisitor {
public:
    CodegenVisitor(const CodegenVisitor &) = delete;
    CodegenVisitor &operator=(const CodegenVisitor &) = delete;

    CodegenVisitor(CodegenVisitor &&) noexcept = delete;
    CodegenVisitor &operator=(CodegenVisitor &&) noexcept = delete;

    virtual ~CodegenVisitor() = default;

    /// Visit a SourceFile AST node.
    /// \param source_file The SourceFile being visited.
    virtual llvm::Value *codegen(const SourceFile &source_file) = 0;

    /// Visit a Identifier AST node.
    /// \param identifier The Identifier being visited.
    virtual llvm::Value *codegen(const Identifier &identifier) = 0;

    /// Visit a Type AST node.
    /// \param type The Type being visited.
    virtual llvm::Value *codegen(const Type &type) = 0;

    /// Visit a FuncDecl AST node.
    /// \param func_decl The FuncDecl being visited.
    virtual llvm::Value *codegen(const FuncDecl &func_decl) = 0;

    /// Visit a IntegerLiteralExpr AST node.
    /// \param integer_literal_expr The integerLiteralExpr being visited.
    virtual llvm::Value *codegen(const IntegerLiteralExpr &integer_literal_expr) = 0;

    /// Visit a Block AST node.
    /// \param block The Block being visited.
    virtual llvm::Value *codegen(const Block &block) = 0;

    /// Visit a ReturnStmt AST node.
    /// \param return_stmt The ReturnStmt being visited.
    virtual llvm::Value *codegen(const ReturnStmt &return_stmt) = 0;

protected:
    CodegenVisitor() = default;
};

} // namespace sodium

#endif // SODIUM_NAC_CODEGEN_CODEGEN_VISITOR_H
