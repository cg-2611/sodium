#include "sodium/nac/ast/type.h"

#include <string_view>

#include "sodium/nac/ast/ast_node.h"
#include "sodium/nac/ast/ast_visitor.h"

namespace sodium {

Type::Type(std::string_view name) : ASTNode(ASTNodeKind::TYPE), name_(name) {}

void Type::accept(ASTVisitor *visitor) const {
    visitor->visit(this);
}

std::string_view Type::name() const noexcept {
    return name_;
}

} // namespace sodium
