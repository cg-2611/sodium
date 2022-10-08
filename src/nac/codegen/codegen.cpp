#include "sodium/nac/codegen/codegen.h"

#include <memory>
#include <utility>

#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Value.h"
#include "llvm/IR/Verifier.h"
#include "llvm/Target/TargetMachine.h"

#include "sodium/nac/ast/ast.h"
#include "sodium/nac/ast/decl.h"
#include "sodium/nac/ast/expr.h"
#include "sodium/nac/ast/identifier.h"
#include "sodium/nac/ast/source_file.h"
#include "sodium/nac/ast/stmt.h"
#include "sodium/nac/ast/type.h"
#include "sodium/nac/codegen/codegen_diagnostics.h"
#include "sodium/nac/diagnostics/diagnostic_engine.h"

namespace sodium {

Codegen::Codegen(const AST &ast, DiagnosticEngine &diagnostics)
        : ast_(ast),
          diagnostics_(diagnostics),
          context_(std::make_unique<llvm::LLVMContext>()),
          builder_(std::make_unique<llvm::IRBuilder<>>(*context_)),
          module_(std::make_unique<llvm::Module>("module", *context_)) {}

llvm::Module *Codegen::generate() {
    ast_.root()->accept(*this);

    return module_.get();
}

llvm::Value *Codegen::codegen(const SourceFile &source_file) {
    for (auto &&decl : source_file.decls()) {
        if (decl->decl_kind() == DeclKind::FUNC) {
            decl->accept(*this);
        }
    }

    return nullptr;
}

llvm::Value *Codegen::codegen([[maybe_unused]] const Identifier &identifier) {
    return nullptr;
}

llvm::Value *Codegen::codegen([[maybe_unused]] const Type &type) {
    return nullptr;
}

llvm::Value *Codegen::codegen(const FuncDecl &func_decl) {
    // create a function type for a function that takes no arguments and returns an int
    llvm::FunctionType *func_type = llvm::FunctionType::get(int_type(), {}, false);

    llvm::Function *func =
        llvm::Function::Create(func_type, llvm::Function::ExternalLinkage, func_decl.name()->value(), module_.get());

    if (!func) {
        return nullptr;
    }

    llvm::BasicBlock *body = llvm::BasicBlock::Create(*context_, "entry", func);
    builder_->SetInsertPoint(body);

    llvm::Value *return_value = func_decl.body()->accept(*this);
    if (!return_value) {
        auto codegen_error =
            std::make_unique<CodegenError>(CodegenErrorKind::NO_RETURN_EXPRESSION, func_decl.body()->range().end());
        diagnostics_.diagnose(std::move(codegen_error));

        func->eraseFromParent();
        return nullptr;
    }

    builder_->CreateRet(return_value);

    llvm::verifyFunction(*func);

    return func;
}

llvm::Value *Codegen::codegen(const IntegerLiteralExpr &integer_literal_expr) {
    return llvm::ConstantInt::getSigned(int_type(), integer_literal_expr.value());
}

llvm::Value *Codegen::codegen(const Block &block) {
    if (block.stmts().empty()) {
        auto codegen_error = std::make_unique<CodegenError>(CodegenErrorKind::NO_RETURN_STATEMENT, block.range().end());
        diagnostics_.diagnose(std::move(codegen_error));
        return nullptr;
    }

    // get the last statement in the block
    const size_t STMT_COUNT = block.stmts().size();
    auto *return_stmt = block.stmts()[STMT_COUNT - 1].get();

    if (!return_stmt || return_stmt->stmt_kind() != StmtKind::RETURN) {
        auto codegen_error = std::make_unique<CodegenError>(CodegenErrorKind::NO_RETURN_STATEMENT, block.range().end());
        diagnostics_.diagnose(std::move(codegen_error));

        return nullptr;
    }

    return return_stmt->accept(*this);
}

llvm::Value *Codegen::codegen(const ReturnStmt &return_stmt) {
    if (!return_stmt.expr()) {
        auto codegen_error =
            std::make_unique<CodegenError>(CodegenErrorKind::NO_RETURN_EXPRESSION, return_stmt.range().end());
        diagnostics_.diagnose(std::move(codegen_error));
        return nullptr;
    }

    return return_stmt.expr()->accept(*this);
}

llvm::Type *Codegen::int_type() {
    return llvm::Type::getInt32Ty(*context_);
}

} // namespace sodium
