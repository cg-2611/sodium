#include "sodium/nac/ast/type.h"

#include <string>
#include <utility>

#include "llvm/IR/Value.h"

#include "sodium/nac/ast/ast_node.h"
#include "sodium/nac/ast/ast_visitor.h"
#include "sodium/nac/basic/source_range.h"
#include "sodium/nac/codegen/codegen_visitor.h"

namespace sodium {

Type::Type(std::string name, SourceRange range) : ASTNode(ASTNodeKind::TYPE, range), name_(std::move(name)) {}

void Type::accept(ASTVisitor &visitor) const {
    visitor.visit(*this);
}

llvm::Value *Type::accept(CodegenVisitor &visitor) const {
    return visitor.codegen(*this);
}

const std::string &Type::name() const {
    return name_;
}

} // namespace sodium
