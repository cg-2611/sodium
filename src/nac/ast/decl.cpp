#include "sodium/nac/ast/decl.h"

#include <memory>
#include <utility>

#include "sodium/nac/ast/ast_node.h"
#include "sodium/nac/ast/ast_visitor.h"
#include "sodium/nac/ast/identifier.h"
#include "sodium/nac/ast/stmt.h"
#include "sodium/nac/ast/type.h"
#include "sodium/nac/basic/source_range.h"

namespace sodium {

Decl::Decl(DeclKind kind, SourceRange range) : ASTNode(ASTNodeKind::DECL, range), kind_(kind) {}

DeclKind Decl::decl_kind() const {
    return kind_;
}

FuncDecl::FuncDecl(std::unique_ptr<FuncSignature> signature, std::unique_ptr<Block> body, SourceRange range)
        : Decl(DeclKind::FUNC, SourceRange(range)),
          signature_(std::move(signature)),
          body_(std::move(body)) {}

void FuncDecl::accept(ASTVisitor &visitor) const {
    visitor.visit(*this);
}

FuncSignature *FuncDecl::signature() const {
    return signature_.get();
}

Block *FuncDecl::body() const {
    return body_.get();
}

FuncSignature::FuncSignature(std::unique_ptr<Identifier> name, std::unique_ptr<ParameterList> parameter_list,
                             std::unique_ptr<Type> return_type, SourceRange range)
        : ASTNode(ASTNodeKind::FUNC_SIGNATURE, range),
          name_(std::move(name)),
          parameter_list_(std::move(parameter_list)),
          return_type_(std::move(return_type)) {}

void FuncSignature::accept(ASTVisitor &visitor) const {
    visitor.visit(*this);
}

Identifier *FuncSignature::name() const {
    return name_.get();
}

ParameterList *FuncSignature::parameter_list() const {
    return parameter_list_.get();
}

Type *FuncSignature::return_type() const {
    return return_type_.get();
}

ParameterList::ParameterList(SourceRange range) : ASTNode(ASTNodeKind::PARAMETER_LIST, range) {}

void ParameterList::accept(ASTVisitor &visitor) const {
    visitor.visit(*this);
}

} // namespace sodium
