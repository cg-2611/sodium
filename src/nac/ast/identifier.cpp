#include "sodium/nac/ast/identifier.h"

#include <string>
#include <utility>

#include "llvm/IR/Value.h"

#include "sodium/nac/ast/ast_node.h"
#include "sodium/nac/ast/ast_visitor.h"
#include "sodium/nac/basic/source_range.h"
#include "sodium/nac/codegen/codegen_visitor.h"

namespace sodium {

Identifier::Identifier(std::string value, SourceRange range)
        : ASTNode(ASTNodeKind::IDENTIFIER, range),
          value_(std::move(value)) {}

void Identifier::accept(ASTVisitor &visitor) const {
    visitor.visit(*this);
}

llvm::Value *Identifier::accept(CodegenVisitor &visitor) const {
    return visitor.codegen(*this);
}

const std::string &Identifier::value() const {
    return value_;
}

} // namespace sodium
