#include "sodium/nac/ast/decl.h"

#include <memory>
#include <utility>

#include "sodium/nac/ast/ast_node.h"
#include "sodium/nac/ast/ast_visitor.h"
#include "sodium/nac/ast/identifier.h"
#include "sodium/nac/ast/parameter_list.h"
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

Signature::Signature(std::unique_ptr<Identifier> name, std::unique_ptr<ParameterList> parameters,
                     std::unique_ptr<Type> returnType)
        : ASTNode(ASTNodeKind::SIGNATURE),
          name_(std::move(name)),
          parameters_(std::move(parameters)),
          returnType_(std::move(returnType)) {}

void Signature::accept(ASTVisitor *visitor) const {
    name_->accept(visitor);
    parameters_->accept(visitor);
    returnType_->accept(visitor);
    visitor->visit(this);
}

Identifier *Signature::name() const noexcept {
    return name_.get();
}

ParameterList *Signature::parameters() const noexcept {
    return parameters_.get();
}

Type *Signature::returnType() const noexcept {
    return returnType_.get();
}

FuncDecl::FuncDecl(std::unique_ptr<Signature> signature, std::unique_ptr<Block> body)
        : Decl(DeclKind::FUNCTION),
          signature_(std::move(signature)),
          body_(std::move(body)) {}

void FuncDecl::accept(ASTVisitor *visitor) const {
    signature_->accept(visitor);
    body_->accept(visitor);
    visitor->visit(this);
}

Signature *FuncDecl::signature() const noexcept {
    return signature_.get();
}

Block *FuncDecl::body() const noexcept {
    return body_.get();
}

} // namespace sodium
