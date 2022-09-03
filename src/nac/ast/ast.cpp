#include "sodium/nac/ast/ast.h"

#include <memory>
#include <utility>

#include "sodium/nac/ast/ast_node.h"

namespace sodium {

AST::AST(std::unique_ptr<ASTNode> root) : root_(std::move(root)) {}

ASTNode *AST::root() const noexcept {
    return root_.get();
}

} // namespace sodium
