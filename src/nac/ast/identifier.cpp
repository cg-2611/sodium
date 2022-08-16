#include "sodium/nac/ast/identifier.h"

#include <string_view>

#include "sodium/nac/ast/ast_node.h"
#include "sodium/nac/ast/ast_visitor.h"

namespace sodium {

Identifier::Identifier(std::string_view value) : ASTNode(ASTNodeKind::IDENTIFIER), value_(value) {}

void Identifier::accept(ASTVisitor *visitor) const {
    visitor->visit(this);
}

std::string_view Identifier::value() const noexcept {
    return value_;
}

} // namespace sodium
