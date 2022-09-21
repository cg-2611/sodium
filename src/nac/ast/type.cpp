#include "sodium/nac/ast/type.h"

#include <string>
#include <utility>

#include "sodium/nac/ast/ast_node.h"
#include "sodium/nac/ast/ast_visitor.h"
#include "sodium/nac/basic/source_range.h"

namespace sodium {

Type::Type(std::string name, SourceRange range) : ASTNode(ASTNodeKind::TYPE, range), name_(std::move(name)) {}

void Type::accept(ASTVisitor &visitor) const {
    visitor.visit(*this);
}

const std::string &Type::name() const {
    return name_;
}

} // namespace sodium
