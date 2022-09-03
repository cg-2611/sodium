#include "sodium/nac/ast/decl.h"

#include <memory>
#include <utility>

#include "sodium/nac/ast/ast_node.h"
#include "sodium/nac/ast/ast_visitor.h"
#include "sodium/nac/ast/identifier.h"
#include "sodium/nac/ast/stmt.h"
#include "sodium/nac/ast/type.h"

namespace sodium {

Decl::Decl(DeclKind kind) : ASTNode(ASTNodeKind::DECL), kind_(kind) {}

void Decl::accept(ASTVisitor *visitor) const {
    visitor->visit(this);
}

DeclKind Decl::declKind() const noexcept {
    return kind_;
}

FuncDecl::FuncDecl(std::unique_ptr<FunctionSignature> signature, std::unique_ptr<Block> body)
        : Decl(DeclKind::FUNCTION),
          signature_(std::move(signature)),
          body_(std::move(body)) {}

void FuncDecl::accept(ASTVisitor *visitor) const {
    signature_->accept(visitor);
    body_->accept(visitor);
    visitor->visit(this);
}

FunctionSignature *FuncDecl::signature() const noexcept {
    return signature_.get();
}

Block *FuncDecl::body() const noexcept {
    return body_.get();
}

FunctionSignature::FunctionSignature(std::unique_ptr<Identifier> name, std::unique_ptr<ParameterList> parameterList,
                                     std::unique_ptr<Type> returnType)
        : Decl(DeclKind::FUNCTION_SIGNATURE),
          name_(std::move(name)),
          parameterList_(std::move(parameterList)),
          returnType_(std::move(returnType)) {}

void FunctionSignature::accept(ASTVisitor *visitor) const {
    name_->accept(visitor);
    parameterList_->accept(visitor);
    returnType_->accept(visitor);
    visitor->visit(this);
}

Identifier *FunctionSignature::name() const noexcept {
    return name_.get();
}

ParameterList *FunctionSignature::parameterList() const noexcept {
    return parameterList_.get();
}

Type *FunctionSignature::returnType() const noexcept {
    return returnType_.get();
}

ParameterList::ParameterList() : Decl(DeclKind::PARAMETER_LIST) {}

void ParameterList::accept(ASTVisitor *visitor) const {
    visitor->visit(this);
}

} // namespace sodium
