#include "sodium/nac/ast/ast_node.h"

namespace sodium {

ASTNode::ASTNode(ASTNodeKind kind) : kind_(kind) {}

ASTNodeKind ASTNode::nodeKind() const noexcept {
    return kind_;
}

} // namespace sodium
