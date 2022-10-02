#include "sodium/nac/ast/decl.h"

#include <memory>
#include <utility>

#include "llvm/IR/Value.h"

#include "sodium/nac/ast/ast_node.h"
#include "sodium/nac/ast/ast_visitor.h"
#include "sodium/nac/ast/identifier.h"
#include "sodium/nac/ast/stmt.h"
#include "sodium/nac/ast/type.h"
#include "sodium/nac/basic/source_range.h"
#include "sodium/nac/codegen/codegen_visitor.h"

namespace sodium {

Decl::Decl(DeclKind kind, SourceRange range) : ASTNode(ASTNodeKind::DECL, range), kind_(kind) {}

DeclKind Decl::decl_kind() const {
    return kind_;
}

FuncDecl::FuncDecl(std::unique_ptr<Identifier> name, std::unique_ptr<Type> return_type, std::unique_ptr<Block> body,
                   SourceRange range)
        : Decl(DeclKind::FUNC, range),
          name_(std::move(name)),
          return_type_(std::move(return_type)),
          body_(std::move(body)) {}

void FuncDecl::accept(ASTVisitor &visitor) const {
    visitor.visit(*this);
}

llvm::Value *FuncDecl::accept(CodegenVisitor &visitor) const {
    return visitor.codegen(*this);
}

Identifier *FuncDecl::name() const {
    return name_.get();
}

Type *FuncDecl::return_type() const {
    return return_type_.get();
}

Block *FuncDecl::body() const {
    return body_.get();
}

} // namespace sodium
