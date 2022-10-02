#ifndef SODIUM_NAC_CODEGEN_CODEGEN_H
#define SODIUM_NAC_CODEGEN_CODEGEN_H

#include <memory>

#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"

#include "sodium/nac/codegen/codegen_visitor.h"
#include "sodium/nac/diagnostics/diagnostic_engine.h"

namespace llvm {

class Module;
class Type;
class Value;

} // namespace llvm

namespace sodium {

class AST;

/// Used to generate  the LLVM IR of the AST using the visitor pattern and CodegenVisitor interface.
class Codegen : public CodegenVisitor {
public:
    /// Constructor for Codegen.
    /// \param AST The AST to generate LLVM IR for.
    /// \param diagnostics The DiagnosticEngine to report errors to.
    Codegen(const AST &ast, DiagnosticEngine &diagnostics);

    /// Generate the LLVM IR for the AST.
    void generate();

    /// Print the generated LLVM IR.
    void print_llvm_ir() const;

    /// Generate LLVM IR for a FuncDecl.
    /// \param func_decl The FuncDecl to generate the LLVM IR for.
    /// \return The generated llvm::Function.
    llvm::Value *codegen(const FuncDecl &func_decl) override;

    /// Generate LLVM IR for a IntegerLiteralExpr.
    /// \param integer_literal_expr The IntegerLiteralExpr to generate the LLVM IR for.
    /// \return The generated llvm::ConstantInt.
    llvm::Value *codegen(const IntegerLiteralExpr &integer_literal_expr) override;

    /// Generate LLVM IR for a Block.
    /// \param block The Block to generate the LLVM IR for.
    /// \return The llvm::Value * for the last statement in the block.
    llvm::Value *codegen(const Block &block) override;

    /// Generate LLVM IR for a ReturnStmt.
    /// \param return_stmt The ReturnStmt to generate the LLVM IR for.
    /// \return The llvm::Value * of the expression being returned.
    llvm::Value *codegen(const ReturnStmt &return_stmt) override;

private:
    const AST &ast_;
    DiagnosticEngine &diagnostics_;

    std::unique_ptr<llvm::LLVMContext> context_;
    std::unique_ptr<llvm::IRBuilder<>> builder_;
    std::unique_ptr<llvm::Module> module_;

    llvm::Value *codegen(const SourceFile &source_file) override;
    llvm::Value *codegen(const Identifier &identifier) override;
    llvm::Value *codegen(const Type &type) override;

    llvm::Type *int_type(); // returns the int32 type from context_
};

} // namespace sodium

#endif // SODIUM_NAC_CODEGEN_CODEGEN_H
