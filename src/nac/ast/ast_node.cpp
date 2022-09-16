#include "sodium/nac/ast/ast_node.h"

#include "sodium/nac/ast/ast_visitor.h"
#include "sodium/nac/basic/source_range.h"

namespace sodium {

ASTNode::ASTNode(ASTNodeKind kind, SourceRange range) : kind_(kind), range_(range) {}

ASTNodeKind ASTNode::node_kind() const {
    return kind_;
}

SourceRange ASTNode::range() const {
    return range_;
}

} // namespace sodium
